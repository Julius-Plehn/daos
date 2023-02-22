//
// (C) Copyright 2019-2023 Intel Corporation.
//
// SPDX-License-Identifier: BSD-2-Clause-Patent
//

package storage

import (
	"encoding/json"
	"strings"
	"testing"

	"github.com/google/go-cmp/cmp"
	"github.com/pkg/errors"
	"gopkg.in/yaml.v2"

	"github.com/daos-stack/daos/src/control/common"
	"github.com/daos-stack/daos/src/control/common/test"
	"github.com/daos-stack/daos/src/control/lib/hardware"
)

func defConfigCmpOpts() cmp.Options {
	return cmp.Options{
		cmp.Comparer(func(x, y *BdevDeviceList) bool {
			if x == nil && y == nil {
				return true
			}
			return x.Equals(y)
		}),
	}
}

func TestStorage_BdevDeviceList_Devices(t *testing.T) {
	for name, tc := range map[string]struct {
		list      *BdevDeviceList
		expResult []string
	}{
		"nil": {
			expResult: []string{},
		},
		"empty": {
			list:      &BdevDeviceList{},
			expResult: []string{},
		},
		"string set": {
			list: &BdevDeviceList{
				stringBdevSet: common.NewStringSet("one", "two"),
			},
			expResult: []string{"one", "two"},
		},
		"PCI addresses": {
			list: &BdevDeviceList{
				PCIAddressSet: *hardware.MustNewPCIAddressSet(
					"0000:01:01.0",
					"0000:02:02.0",
				),
			},
			expResult: []string{"0000:01:01.0", "0000:02:02.0"},
		},
	} {
		t.Run(name, func(t *testing.T) {
			result := tc.list.Devices()

			if diff := cmp.Diff(tc.expResult, result); diff != "" {
				t.Fatalf("(-want +got):\n%s", diff)
			}
		})
	}
}

func TestStorage_BdevDeviceList(t *testing.T) {
	for name, tc := range map[string]struct {
		devices    []string
		expList    *BdevDeviceList
		expYamlStr string
		expJSONStr string
		expErr     error
	}{
		"empty": {
			expList:    &BdevDeviceList{},
			expYamlStr: "[]\n",
			expJSONStr: "[]",
		},
		"valid pci addresses": {
			devices: []string{"0000:81:00.0", "0000:82:00.0"},
			expList: &BdevDeviceList{
				PCIAddressSet: func() hardware.PCIAddressSet {
					set, err := hardware.NewPCIAddressSetFromString("0000:81:00.0 0000:82:00.0")
					if err != nil {
						panic(err)
					}
					return *set
				}(),
			},
			expYamlStr: `
- 0000:81:00.0
- 0000:82:00.0
`,
			expJSONStr: `["0000:81:00.0","0000:82:00.0"]`,
		},
		"non-pci devices": {
			devices: []string{"/dev/block0", "/dev/block1"},
			expList: &BdevDeviceList{
				stringBdevSet: common.NewStringSet("/dev/block0", "/dev/block1"),
			},
			expYamlStr: `
- /dev/block0
- /dev/block1
`,
			expJSONStr: `["/dev/block0","/dev/block1"]`,
		},
		"invalid pci device": {
			devices: []string{"0000:8g:00.0"},
			expErr:  errors.New("unable to parse \"0000:8g:00.0\""),
		},
		"mixed pci and non-pci devices": {
			devices: []string{"/dev/block0", "0000:81:00.0"},
			expErr:  errors.New("mix"),
		},
		"duplicate pci device": {
			devices: []string{"0000:81:00.0", "0000:81:00.0"},
			expErr:  errors.New("duplicate"),
		},
		"duplicate non-pci device": {
			devices: []string{"/dev/block0", "/dev/block0"},
			expErr:  errors.New("duplicate"),
		},
	} {
		t.Run(name, func(t *testing.T) {
			list, err := NewBdevDeviceList(tc.devices...)
			test.CmpErr(t, tc.expErr, err)
			if tc.expErr != nil {
				return
			}

			if diff := cmp.Diff(tc.expList, list, defConfigCmpOpts()...); diff != "" {
				t.Fatalf("bad list (-want +got):\n%s", diff)
			}

			yamlData, err := yaml.Marshal(list)
			if err != nil {
				t.Fatal(err)
			}
			if diff := cmp.Diff(strings.TrimLeft(tc.expYamlStr, "\n"), string(yamlData)); diff != "" {
				t.Fatalf("bad yaml (-want +got):\n%s", diff)
			}

			jsonData, err := json.Marshal(list)
			if err != nil {
				t.Fatal(err)
			}
			if diff := cmp.Diff(strings.TrimLeft(tc.expJSONStr, "\n"), string(jsonData)); diff != "" {
				t.Fatalf("bad json (-want +got):\n%s", diff)
			}
		})
	}
}

func TestStorage_BdevDeviceList_FromYAML(t *testing.T) {
	for name, tc := range map[string]struct {
		input   string
		expList *BdevDeviceList
		expErr  error
	}{
		"empty": {
			input:   "[]\n",
			expList: &BdevDeviceList{},
		},
		"valid pci addresses": {
			input: `["0000:81:00.0","0000:82:00.0"]`,
			expList: &BdevDeviceList{
				PCIAddressSet: func() hardware.PCIAddressSet {
					set, err := hardware.NewPCIAddressSetFromString("0000:81:00.0 0000:82:00.0")
					if err != nil {
						panic(err)
					}
					return *set
				}(),
			},
		},
		"non-pci devices": {
			input: `
- /dev/block0
- /dev/block1
`,
			expList: &BdevDeviceList{
				stringBdevSet: common.NewStringSet("/dev/block0", "/dev/block1"),
			},
		},
		"mixed pci and non-pci devices": {
			input:  `["/dev/block0", "0000:81:00.0"]`,
			expErr: errors.New("mix"),
		},
	} {
		t.Run(name, func(t *testing.T) {
			list := &BdevDeviceList{}
			err := yaml.Unmarshal([]byte(tc.input), list)
			test.CmpErr(t, tc.expErr, err)
			if tc.expErr != nil {
				return
			}

			if diff := cmp.Diff(tc.expList, list, defConfigCmpOpts()...); diff != "" {
				t.Fatalf("bad list (-want +got):\n%s", diff)
			}
		})
	}
}

func TestStorage_BdevDeviceList_FromJSON(t *testing.T) {
	for name, tc := range map[string]struct {
		input   string
		expList *BdevDeviceList
		expErr  error
	}{
		"empty": {
			input:   "[]\n",
			expList: &BdevDeviceList{},
		},
		"valid pci addresses": {
			input: `["0000:81:00.0","0000:82:00.0"]`,
			expList: &BdevDeviceList{
				PCIAddressSet: func() hardware.PCIAddressSet {
					set, err := hardware.NewPCIAddressSetFromString("0000:81:00.0 0000:82:00.0")
					if err != nil {
						panic(err)
					}
					return *set
				}(),
			},
		},
		"non-pci devices": {
			input: `["/dev/block0","/dev/block1"]`,
			expList: &BdevDeviceList{
				stringBdevSet: common.NewStringSet("/dev/block0", "/dev/block1"),
			},
		},
		"mixed pci and non-pci devices": {
			input:  `["/dev/block0", "0000:81:00.0"]`,
			expErr: errors.New("mix"),
		},
	} {
		t.Run(name, func(t *testing.T) {
			list := &BdevDeviceList{}
			err := json.Unmarshal([]byte(tc.input), list)
			test.CmpErr(t, tc.expErr, err)
			if tc.expErr != nil {
				return
			}

			if diff := cmp.Diff(tc.expList, list, defConfigCmpOpts()...); diff != "" {
				t.Fatalf("bad list (-want +got):\n%s", diff)
			}
		})
	}
}

func TestStorage_parsePCIBusRange(t *testing.T) {
	for name, tc := range map[string]struct {
		rangeStr string
		bitSize  int
		expBegin uint8
		expEnd   uint8
		expErr   error
	}{
		"hexadecimal": {
			rangeStr: "0x80-0x8f",
			expBegin: 0x80,
			expEnd:   0x8f,
		},
		"incorrect hexadecimal": {
			rangeStr: "0x8g-0x8f",
			expErr:   errors.New("parsing \"0x8g\""),
		},
		"hexadecimal upper": {
			rangeStr: "0x80-0x8F",
			expBegin: 0x80,
			expEnd:   0x8F,
		},
		"decimal": {
			rangeStr: "128-143",
			expBegin: 0x80,
			expEnd:   0x8F,
		},
		"bad range": {
			rangeStr: "128-143-0",
			expErr:   errors.New("invalid busid range \"128-143-0\""),
		},
		"reverse range": {
			rangeStr: "143-0",
			expErr:   errors.New("invalid busid range \"143-0\""),
		},
		"bad separator": {
			rangeStr: "143:0",
			expErr:   errors.New("invalid busid range \"143:0\""),
		},
		"hexadecimal; no prefix": {
			rangeStr: "00-5d",
			bitSize:  8,
			expErr:   errors.New("invalid syntax"),
		},
		"hexadecimal; bit-size exceeded": {
			rangeStr: "0x000-0x5dd",
			bitSize:  8,
			expErr:   errors.New("value out of range"),
		},
	} {
		t.Run(name, func(t *testing.T) {
			begin, end, err := parsePCIBusRange(tc.rangeStr, tc.bitSize)
			test.CmpErr(t, tc.expErr, err)
			if tc.expErr != nil {
				return
			}

			test.AssertEqual(t, tc.expBegin, begin, "bad beginning limit")
			test.AssertEqual(t, tc.expEnd, end, "bad ending limit")
		})
	}
}

func TestStorage_BdevDeviceRoles_FromYAML(t *testing.T) {
	for name, tc := range map[string]struct {
		input           string
		expCfgs         TierConfigs
		expUnmarshalErr error
		expValidateErr  error
	}{
		"section missing": {
			input:          ``,
			expValidateErr: errors.New("no storage tiers"),
		},
		"unspecified roles; implicit role assignment; one bdev tier": {
			input: `
storage:
-
  class: ram
  scm_size: 16
  scm_mount: /mnt/daos
-
  class: nvme
  bdev_list: [0000:80:00.0]`,
			expCfgs: TierConfigs{
				NewTierConfig().
					WithStorageClass("ram").
					WithScmRamdiskSize(16).
					WithScmMountPoint("/mnt/daos"),
				NewTierConfig().
					WithTier(1).
					WithStorageClass("nvme").
					WithBdevDeviceList("0000:80:00.0").
					WithBdevDeviceRoles(BdevRoleAll),
			},
		},
		"unspecified roles; implicit role assignment; two bdev tiers": {
			input: `
storage:
-
  class: ram
  scm_size: 16
  scm_mount: /mnt/daos
-
  class: nvme
  bdev_list: [0000:80:00.0]
-
  class: nvme
  bdev_list: [0000:81:00.0,0000:82:00.0]`,
			expCfgs: TierConfigs{
				NewTierConfig().
					WithStorageClass("ram").
					WithScmRamdiskSize(16).
					WithScmMountPoint("/mnt/daos"),
				NewTierConfig().
					WithTier(1).
					WithStorageClass("nvme").
					WithBdevDeviceList("0000:80:00.0").
					WithBdevDeviceRoles(BdevRoleWAL),
				NewTierConfig().
					WithTier(2).
					WithStorageClass("nvme").
					WithBdevDeviceList("0000:81:00.0", "0000:82:00.0").
					WithBdevDeviceRoles(BdevRoleMeta | BdevRoleData),
			},
		},
		"unspecified roles; implicit role assignment; three bdev tiers": {
			input: `
storage:
-
  class: ram
  scm_size: 16
  scm_mount: /mnt/daos
-
  class: nvme
  bdev_list: [0000:80:00.0]
-
  class: nvme
  bdev_list: [0000:81:00.0,0000:82:00.0]
-
  class: nvme
  bdev_list: [0000:83:00.0,0000:84:00.0]`,
			expCfgs: TierConfigs{
				NewTierConfig().
					WithStorageClass("ram").
					WithScmRamdiskSize(16).
					WithScmMountPoint("/mnt/daos"),
				NewTierConfig().
					WithTier(1).
					WithStorageClass("nvme").
					WithBdevDeviceList("0000:80:00.0").
					WithBdevDeviceRoles(BdevRoleWAL),
				NewTierConfig().
					WithTier(2).
					WithStorageClass("nvme").
					WithBdevDeviceList("0000:81:00.0", "0000:82:00.0").
					WithBdevDeviceRoles(BdevRoleMeta),
				NewTierConfig().
					WithTier(3).
					WithStorageClass("nvme").
					WithBdevDeviceList("0000:83:00.0", "0000:84:00.0").
					WithBdevDeviceRoles(BdevRoleData),
			},
		},
		"unspecified roles; implicit role assignment; four bdev tiers": {
			input: `
storage:
-
  class: ram
  scm_size: 16
  scm_mount: /mnt/daos
-
  class: nvme
  bdev_list: [0000:80:00.0]
-
  class: nvme
  bdev_list: [0000:81:00.0,0000:82:00.0]
-
  class: nvme
  bdev_list: [0000:83:00.0,0000:84:00.0]
-
  class: nvme
  bdev_list: [0000:85:00.0,0000:86:00.0]`,
			expCfgs: TierConfigs{
				NewTierConfig().
					WithStorageClass("ram").
					WithScmRamdiskSize(16).
					WithScmMountPoint("/mnt/daos"),
				NewTierConfig().
					WithTier(1).
					WithStorageClass("nvme").
					WithBdevDeviceList("0000:80:00.0").
					WithBdevDeviceRoles(BdevRoleWAL),
				NewTierConfig().
					WithTier(2).
					WithStorageClass("nvme").
					WithBdevDeviceList("0000:81:00.0", "0000:82:00.0").
					WithBdevDeviceRoles(BdevRoleMeta),
				NewTierConfig().
					WithTier(3).
					WithStorageClass("nvme").
					WithBdevDeviceList("0000:83:00.0", "0000:84:00.0").
					WithBdevDeviceRoles(BdevRoleData),
				NewTierConfig().
					WithTier(4).
					WithStorageClass("nvme").
					WithBdevDeviceList("0000:85:00.0", "0000:86:00.0").
					WithBdevDeviceRoles(BdevRoleData),
			},
		},
		"specified roles; explicit role assignment": {
			input: `
storage:
-
  class: ram
  scm_size: 16
  scm_mount: /mnt/daos
-
  class: nvme
  bdev_list: [0000:80:00.0]
  bdev_roles: [meta,wal]
-
  class: nvme
  bdev_list: [0000:81:00.0,0000:82:00.0]
  bdev_roles: [data]`,
			expCfgs: TierConfigs{
				NewTierConfig().
					WithStorageClass("ram").
					WithScmRamdiskSize(16).
					WithScmMountPoint("/mnt/daos"),
				NewTierConfig().
					WithTier(1).
					WithStorageClass("nvme").
					WithBdevDeviceList("0000:80:00.0").
					WithBdevDeviceRoles(BdevRoleMeta | BdevRoleWAL),
				NewTierConfig().
					WithTier(2).
					WithStorageClass("nvme").
					WithBdevDeviceList("0000:81:00.0", "0000:82:00.0").
					WithBdevDeviceRoles(BdevRoleData),
			},
		},
		"specified roles; alternative format": {
			input: `
storage:
-
  class: ram
  scm_size: 16
  scm_mount: /mnt/daos
-
  class: nvme
  bdev_list:
  - 0000:80:00.0
  bdev_roles:
  - meta
  - wal
-
  class: nvme
  bdev_list:
  - 0000:81:00.0
  - 0000:82:00.0
  bdev_roles:
  - data`,
			expCfgs: TierConfigs{
				NewTierConfig().
					WithStorageClass("ram").
					WithScmRamdiskSize(16).
					WithScmMountPoint("/mnt/daos"),
				NewTierConfig().
					WithTier(1).
					WithStorageClass("nvme").
					WithBdevDeviceList("0000:80:00.0").
					WithBdevDeviceRoles(BdevRoleMeta | BdevRoleWAL),
				NewTierConfig().
					WithTier(2).
					WithStorageClass("nvme").
					WithBdevDeviceList("0000:81:00.0", "0000:82:00.0").
					WithBdevDeviceRoles(BdevRoleData),
			},
		},
		"unrecognized role": {
			input: `
storage:
-
  class: ram
  scm_size: 16
  scm_mount: /mnt/daos
-
  class: nvme
  bdev_list: [0000:80:00.0]
  bdev_roles: [foobar]`,
			expUnmarshalErr: errors.New("unknown option flag"),
		},
		"dcpm class; multiple bdev tiers": {
			input: `
storage:
-
  class: dcpm
  scm_list: [/dev/pmem0]
  scm_mount: /mnt/daos
-
  class: nvme
  bdev_list:
  - 0000:80:00.0
  bdev_roles:
  - meta
  - wal
-
  class: nvme
  bdev_list:
  - 0000:81:00.0
  - 0000:82:00.0
  bdev_roles:
  - data`,
			expValidateErr: FaultBdevConfigMultiTiersWithDCPM,
		},
		"dcpm class; illegal roles": {
			input: `
storage:
-
  class: dcpm
  scm_list: [/dev/pmem0]
  scm_mount: /mnt/daos
-
  class: nvme
  bdev_list: [0000:80:00.0]
  bdev_roles: [meta,wal,data]`,
			expValidateErr: FaultBdevConfigBadNrRoles("WAL", 1, 0),
		},
		"ram class; duplicate wal roles": {
			input: `
storage:
-
  class: ram
  scm_size: 16
  scm_mount: /mnt/daos
-
  class: nvme
  bdev_list: [0000:80:00.0]
  bdev_roles: [meta,wal]
-
  class: nvme
  bdev_list: [0000:81:00.0,0000:82:00.0]
  bdev_roles: [wal,data]`,
			expValidateErr: FaultBdevConfigBadNrRoles("WAL", 2, 1),
		},
		"ram class; duplicate meta roles": {
			input: `
storage:
-
  class: ram
  scm_size: 16
  scm_mount: /mnt/daos
-
  class: nvme
  bdev_list: [0000:80:00.0]
  bdev_roles: [meta,wal]
-
  class: nvme
  bdev_list: [0000:81:00.0,0000:82:00.0]
  bdev_roles: [meta,data]`,
			expValidateErr: FaultBdevConfigBadNrRoles("Meta", 2, 1),
		},
		"ram class; missing data role": {
			input: `
storage:
-
  class: ram
  scm_size: 16
  scm_mount: /mnt/daos
-
  class: nvme
  bdev_list: [0000:80:00.0]
  bdev_roles: [meta,wal]`,
			expValidateErr: FaultBdevConfigBadNrRoles("Data", 0, 1),
		},
		"ram class; tier with unassigned roles": {
			input: `
storage:
-
  class: ram
  scm_size: 16
  scm_mount: /mnt/daos
-
  class: nvme
  bdev_list: [0000:80:00.0]
-
  class: nvme
  bdev_list: [0000:81:00.0,0000:82:00.0]
  bdev_roles: [wal]`,
			expValidateErr: errors.New("tiers are missing role assignments: [1]"),
		},
		"explicit specification of data-only role": {
			input: `
storage:
-
  class: ram
  scm_size: 16
  scm_mount: /mnt/daos
-
  class: nvme
  bdev_list:
  - 0000:80:00.0
  bdev_roles:
  - data`,
			expCfgs: TierConfigs{
				NewTierConfig().
					WithStorageClass("ram").
					WithScmRamdiskSize(16).
					WithScmMountPoint("/mnt/daos"),
				NewTierConfig().
					WithTier(1).
					WithStorageClass("nvme").
					WithBdevDeviceList("0000:80:00.0").
					WithBdevDeviceRoles(BdevRoleData),
			},
		},
	} {
		t.Run(name, func(t *testing.T) {
			cfg := new(Config)
			err := yaml.Unmarshal([]byte(tc.input), cfg)
			test.CmpErr(t, tc.expUnmarshalErr, err)
			if err != nil {
				return
			}

			err = cfg.Validate()
			test.CmpErr(t, tc.expValidateErr, err)
			if err != nil {
				return
			}

			if diff := cmp.Diff(tc.expCfgs, cfg.Tiers, defConfigCmpOpts()...); diff != "" {
				t.Fatalf("bad roles (-want +got):\n%s", diff)
			}
		})
	}
}

func TestStorage_BdevDeviceRoles_ToYAML(t *testing.T) {
	for name, tc := range map[string]struct {
		cfg    *Config
		expOut string
		expErr error
	}{
		"section missing": {
			cfg:    &Config{},
			expOut: "storage: []\n",
		},
		"specified roles": {
			cfg: &Config{
				Tiers: TierConfigs{
					NewTierConfig().
						WithStorageClass("ram").
						WithScmRamdiskSize(16).
						WithScmMountPoint("/mnt/daos"),
					NewTierConfig().
						WithTier(1).
						WithStorageClass("nvme").
						WithBdevDeviceList("0000:80:00.0").
						WithBdevDeviceRoles(BdevRoleAll),
				},
			},
			expOut: `
storage:
- class: ram
  scm_mount: /mnt/daos
  scm_size: 16
- class: nvme
  bdev_list:
  - 0000:80:00.0
  bdev_roles:
  - data
  - meta
  - wal
`,
		},
	} {
		t.Run(name, func(t *testing.T) {
			bytes, err := yaml.Marshal(tc.cfg)
			test.CmpErr(t, tc.expErr, err)
			if tc.expErr != nil {
				return
			}

			if diff := cmp.Diff(strings.TrimLeft(tc.expOut, "\n"), string(bytes),
				defConfigCmpOpts()...); diff != "" {
				t.Fatalf("bad yaml output (-want +got):\n%s", diff)
			}
		})
	}
}

func TestStorage_AccelProps_FromYAML(t *testing.T) {
	for name, tc := range map[string]struct {
		input    string
		expProps AccelProps
		expErr   error
	}{
		"acceleration section missing": {
			input: ``,
		},
		"acceleration section empty": {
			input: `
acceleration:
`,
		},
		"engine unset": {
			input: `
acceleration:
  engine:
`,
			expErr: errors.New("unknown acceleration engine"),
		},
		"engine set empty": {
			input: `
acceleration:
  engine: ""
`,
			expErr: errors.New("unknown acceleration engine"),
		},
		"engine set; default opts": {
			input: `
acceleration:
  engine: spdk
`,
			expProps: AccelProps{
				Engine:  AccelEngineSPDK,
				Options: AccelOptCRCFlag | AccelOptMoveFlag,
			},
		},
		"engine unset; opts set": {
			input: `
acceleration:
  options:
  - move
  - crc
`,
			expProps: AccelProps{
				Engine: AccelEngineNone,
			},
		},
		"engine set; opts set": {
			input: `
acceleration:
  engine: dml
  options:
  - crc
`,
			expProps: AccelProps{
				Engine:  AccelEngineDML,
				Options: AccelOptCRCFlag,
			},
		},
		"engine set; opts all set": {
			input: `
acceleration:
  engine: spdk
  options:
  - crc
  - move
`,
			expProps: AccelProps{
				Engine:  AccelEngineSPDK,
				Options: AccelOptCRCFlag | AccelOptMoveFlag,
			},
		},
		"unrecognized engine": {
			input: `
acceleration:
  engine: native
`,
			expErr: errors.New("unknown acceleration engine"),
		},
		"unrecognized option": {
			input: `
acceleration:
  engine: dml
  options:
  - bar
`,
			expErr: errors.New("unknown option flag"),
		},
	} {
		t.Run(name, func(t *testing.T) {
			cfg := new(Config)
			err := yaml.Unmarshal([]byte(tc.input), cfg)
			test.CmpErr(t, tc.expErr, err)
			if tc.expErr != nil {
				return
			}

			if diff := cmp.Diff(tc.expProps, cfg.AccelProps, defConfigCmpOpts()...); diff != "" {
				t.Fatalf("bad props (-want +got):\n%s", diff)
			}
		})
	}
}

func TestStorage_AccelProps_ToYAML(t *testing.T) {
	for name, tc := range map[string]struct {
		props  AccelProps
		expOut string
		expErr error
	}{
		"nil props": {
			expOut: `
storage: []
`,
		},
		"empty props": {
			expOut: `
storage: []
`,
		},
		"engine set": {
			props: AccelProps{Engine: AccelEngineNone},
			expOut: `
storage: []
acceleration:
  engine: none
`,
		},
		"engine set; default opts": {
			props: AccelProps{
				Engine:  AccelEngineSPDK,
				Options: AccelOptCRCFlag | AccelOptMoveFlag,
			},
			expOut: `
storage: []
acceleration:
  engine: spdk
  options:
  - crc
  - move
`,
		},
	} {
		t.Run(name, func(t *testing.T) {
			cfg := &Config{
				AccelProps: tc.props,
			}

			bytes, err := yaml.Marshal(cfg)
			test.CmpErr(t, tc.expErr, err)
			if tc.expErr != nil {
				return
			}

			if diff := cmp.Diff(strings.TrimLeft(tc.expOut, "\n"), string(bytes), defConfigCmpOpts()...); diff != "" {
				t.Fatalf("bad yaml output (-want +got):\n%s", diff)
			}
		})
	}
}

func TestStorage_ControlMetadata_Directory(t *testing.T) {
	for name, tc := range map[string]struct {
		cm        ControlMetadata
		expResult string
	}{
		"empty": {},
		"path": {
			cm: ControlMetadata{
				Path: "/some/thing",
			},
			expResult: "/some/thing/daos_control",
		},
	} {
		t.Run(name, func(t *testing.T) {
			test.AssertEqual(t, tc.expResult, tc.cm.Directory(), "")
		})
	}
}

func TestStorage_ControlMetadata_EngineDirectory(t *testing.T) {
	for name, tc := range map[string]struct {
		cm        ControlMetadata
		idx       uint
		expResult string
	}{
		"empty": {},
		"path": {
			cm: ControlMetadata{
				Path: "/some/thing",
			},
			idx:       123,
			expResult: "/some/thing/daos_control/engine123",
		},
	} {
		t.Run(name, func(t *testing.T) {
			test.AssertEqual(t, tc.expResult, tc.cm.EngineDirectory(tc.idx), "")
		})
	}
}

func TestStorage_ControlMetadata_HasPath(t *testing.T) {
	for name, tc := range map[string]struct {
		cm        ControlMetadata
		expResult bool
	}{
		"empty": {},
		"path": {
			cm: ControlMetadata{
				Path: "/some/thing",
			},
			expResult: true,
		},
		"path and device": {
			cm: ControlMetadata{
				Path:       "/some/thing",
				DevicePath: "/other/thing",
			},
			expResult: true,
		},
		"device only": {
			cm: ControlMetadata{
				DevicePath: "/some/thing",
			},
		},
	} {
		t.Run(name, func(t *testing.T) {
			test.AssertEqual(t, tc.expResult, tc.cm.HasPath(), "")
		})
	}
}
