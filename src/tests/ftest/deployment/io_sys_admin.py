"""
  (C) Copyright 2018-2024 Intel Corporation.

  SPDX-License-Identifier: BSD-2-Clause-Patent
"""

import time

from data_mover_test_base import DataMoverTestBase
from file_count_test_base import FileCountTestBase
from general_utils import human_to_bytes
from test_utils_pool import check_pool_creation


class IoSysAdmin(DataMoverTestBase, FileCountTestBase):
    # pylint: disable=too-many-ancestors
    """Test Class Description: Test class for large IO tests and some
                               basic system admin operations.
    :avocado: recursive
    """

    def get_free_space(self):
        """Display pool free space."""
        pool_space = self.pool.get_pool_daos_space()
        self.log.info("Free space [SCM, NVMe]: %s", list(pool_space["s_free"]))
        return pool_space["s_free"]

    def test_io_sys_admin(self):
        """
        Test Description: Performs tests to generate large data sets over
                          various middleware, perform various system admin
                          operations, datamover operations.

        :avocado: tags=all,full_regression
        :avocado: tags=hw,medium
        :avocado: tags=deployment,datamover,ior,mdtest
        :avocado: tags=IoSysAdmin,test_io_sys_admin
        """
        # local param
        new_cont_user = self.params.get("user", "/run/container_set_owner/*")
        new_cont_group = self.params.get("group", "/run/container_set_owner/*")

        # Toggle independent steps
        run_step_pool_create_ownership = self.params.get(
            "run_step_pool_create_ownership", "/run/io_sys_admin/*", True)
        run_step_storage_system_query = self.params.get(
            "run_step_storage_system_query", "/run/io_sys_admin/*", True)
        run_step_io = self.params.get(
            "run_step_io", "/run/io_sys_admin/*", True)
        run_step_snapshot = self.params.get(
            "run_step_snapshot", "/run/io_sys_admin/*", True)
        run_test_datamover = self.params.get(
            "run_test_datamover", "/run/io_sys_admin/*", True)

        dmg = self.get_dmg_command()
        daos = self.get_daos_command()

        if run_step_pool_create_ownership:
            self.log_step("Verify pool creation time and container set-owner")
            for idx in range(1, 4):
                pool = self.get_pool(namespace=f"/run/pool_{idx}/", create=False)
                check_pool_creation(self, [pool], 60)
                containers = []
                for cont_idx in range(1, 4):
                    containers.append(
                        self.get_container(pool, namespace=f"/run/container_{cont_idx}/"))
                    containers[-1].set_owner(f"{new_cont_user}@", f"{new_cont_group}@")

                daos.container_list(pool.identifier)
                self.destroy_containers(containers)
                pool.destroy()

        if run_step_storage_system_query:
            self.log_step("Verify storage scan and system query")
            dmg.storage_scan()
            dmg.system_query()
            dmg.system_leader_query()

        if run_step_io:
            self.log_step("Verifying large dataset IO")
            self.run_file_count()

        if run_step_snapshot:
            self.log_step("Verifying snapshot creation and aggregation")
            self.container[-1].create_snap()
            # overwrite the last ior file
            self.ior_cmd.signature.update('456')
            self.processes = self.ior_np
            self.ppn = self.ior_ppn
            self.run_ior_with_pool(create_pool=False, create_cont=False)

            nvme_free_space_before_snap_destroy = self.get_free_space()[1]
            # delete snapshot
            self.container[-1].destroy_snap(epc=self.container[-1].epoch)
            # Now check if the space is returned back.
            counter = 1
            returned_space = self.get_free_space()[1] - nvme_free_space_before_snap_destroy

            data_written = (int(self.ppn) * human_to_bytes(self.ior_cmd.block_size.value))
            while returned_space < int(data_written):
                # try to wait for 4 x 60 secs for aggregation to be completed or
                # else exit the test with a failure.
                if counter > 4:
                    self.log.info(
                        "Free space before snapshot destroy: %s",
                        nvme_free_space_before_snap_destroy)
                    self.log.info(
                        "Free space when test terminated: %s", self.get_free_space()[1])
                    self.fail("Aggregation did not complete as expected")

                time.sleep(60)
                returned_space = self.get_free_space()[1] - nvme_free_space_before_snap_destroy
                counter += 1

        if run_test_datamover:
            self.log_step("Verifying datamover")
            self.log.info("#####Starting FS_COPY Test")
            self.run_dm_activities_with_ior("FS_COPY", self.pool, self.container[-1])
            self.log.info("#####Starting DCP Test")
            self.run_dm_activities_with_ior("DCP", self.pool, self.container[-1])
            self.log.info("#####Starting DSERIAL Test")
            self.run_dm_activities_with_ior("DSERIAL", self.pool, self.container[-1])
            self.log.info("#####Starting CONT_CLONE Test")
            self.run_dm_activities_with_ior("CONT_CLONE", self.pool, self.container[-1])
            self.log.info("#####Completed all Datamover tests")
            self.container.pop(0)
