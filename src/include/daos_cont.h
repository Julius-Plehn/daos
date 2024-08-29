/*
 * (C) Copyright 2020-2024 Intel Corporation.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 */
/**
 * \file
 *
 * DAOS API methods
 */

#ifndef __DAOS_CONT_H__
#define __DAOS_CONT_H__

/** Please ignore (code for back-compatibility) */
#define daos_cont_open daos_cont_open2
/** Please ignore (code for back-compatibility) */
#define daos_cont_destroy daos_cont_destroy2
/** Please ignore (code for back-compatibility) */
#define daos_cont_create daos_cont_create2

#if defined(__cplusplus)
extern "C" {
#endif

#include <daos_security.h>

/**
 * Opens the container for reading only. This flag conflicts with DAOS_COO_RW
 * and DAOS_COO_EX.
 */
#define DAOS_COO_RO		(1U << 0)

/**
 * Opens the container for reading and writing. This flag conflicts with
 * DAOS_COO_RO and DAOS_COO_EX.
 */
#define DAOS_COO_RW		(1U << 1)

/**
 * Opens the container for exclusive reading and writing. This flag conflicts
 * with DAOS_COO_RO and DAOS_COO_RW. The current user must be the owner of the
 * container.
 */
#define DAOS_COO_EX		(1U << 2)

/** Skips the check to see if the pool meets the redundancy factor/level requirements of the
 * container.
 */
#define DAOS_COO_FORCE		(1U << 3)

/** Skips container metadata time updates on DAOS_COO_RO open, and subsequent close */
#define DAOS_COO_RO_MDSTATS	(1U << 4)

/**
 * Before opening the container, evict current user's handles. This flag
 * conflicts with DAOS_COO_EVICT_ALL. This flag can only be used with
 * DAOS_COO_RO or DAOS_COO_RW at the moment.
 *
 * This flag is for recovery purposes and shall not be used by regular
 * applications.
 */
#define DAOS_COO_EVICT		(1U << 5)

/**
 * Before opening the container, evict all handles, including other users'.
 * This flag conflicts with DAOS_COO_EVICT. This flag can only be used with
 * DAOS_COO_EX at the moment. The current user must be the owner of the
 * container.
 *
 * This flag is for recovery purposes and shall not be used by regular
 * applications.
 */
#define DAOS_COO_EVICT_ALL	(1U << 6)

/** Number of bits in the container open mode flag, DAOS_COO_ bits */
#define DAOS_COO_NBITS	(7)

/** Mask for all of the bits in the container open mode flag, DAOS_COO_ bits */
#define DAOS_COO_MASK	((1U << DAOS_COO_NBITS) - 1)

/** The basic IO mode: read-only, read-write or exclusively read-write. */
#define DAOS_COO_IO_BASE_MASK   (DAOS_COO_RO | DAOS_COO_RW | DAOS_COO_EX)

/** Maximum length for container hints */
#define DAOS_CONT_HINT_MAX_LEN	128

/**
 * Generate a rank list from a string with a separator argument. This is a
 * convenience function to generate the rank list required by
 * daos_pool_connect().
 *
 * \param[in]	str	string with the rank list
 * \param[in]	sep	separator of the ranks in \a str.
 *			dmg uses ":" as the separator.
 *
 * \return		allocated rank list that user is responsible to free
 *			with d_rank_list_free().
 */
d_rank_list_t *daos_rank_list_parse(const char *str, const char *sep);

/**
 * Convert a local container handle to global representation data which can be
 * shared with peer processes.
 * If glob->iov_buf is set to NULL, the actual size of the global handle is
 * returned through glob->iov_buf_len.
 * This function does not involve any communication and does not block.
 *
 * \param[in]	coh	valid local container handle to be shared
 * \param[out]	glob	pointer to iov of the buffer to store handle information
 *
 * \return		These values will be returned:
 *			non-blocking mode:
 *			0		Success
 *			-DER_INVAL	Invalid parameter
 *			-DER_NO_HDL	Container handle is nonexistent
 *			-DER_TRUNC	Buffer in \a glob is too short, larger
 *					buffer required. In this case the
 *					required buffer size is returned through
 *					glob->iov_buf_len.
 */
int
daos_cont_local2global(daos_handle_t coh, d_iov_t *glob);

/**
 * Create a local container handle for global representation data.
 *
 * \param[in]	poh	Pool connection handle the container belong to
 * \param[in]	glob	Global (shared) representation of a collective handle
 *			to be extracted
 * \param[out]	coh	Returned local container handle
 *
 * \return		These values will be returned:
 *			non-blocking mode:
 *			0		Success
 *			-DER_INVAL	Invalid parameter
 *			-DER_NO_HDL	Pool handle is nonexistent
 */
int
daos_cont_global2local(daos_handle_t poh, d_iov_t glob, daos_handle_t *coh);

/*
 * Container API
 */

/**
 * Create a new container on the storage pool connected by \a poh.  The label (along with other
 * container properties) can be passed via the \a cont_prop.
 * If no label is specified, a container without any labels will be created. In this case, the only
 * way to identify the newly created container will be via its UUID. It is thus recommended to pass
 * a \a uuid argumnent so that the UUID allocated to the container can be returned to the caller.
 *
 * \param[in]	poh	Pool connection handle.
 * \param[out]	uuid	Optional, pointer to uuid_t to hold the implementation-generated container
 *			UUID.
 * \param[in]	cont_prop
 *			Optional, container properties pointer
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 *
 * \return		These values will be returned by \a ev::ev_error in
 *			non-blocking mode:
 *			0		Success
 *			-DER_INVAL	Invalid parameter
 *			-DER_NO_HDL	Invalid pool handle
 *			-DER_NO_PERM	Permission denied
 *			-DER_UNREACH	network is unreachable
 */
int
daos_cont_create(daos_handle_t poh, uuid_t *uuid, daos_prop_t *cont_prop, daos_event_t *ev);

/**
 * Create a new container with label \a label on the storage pool connected
 * by \a poh. Helper method built over the regular daos_cont_create().
 *
 * \param[in]	poh	Pool connection handle.
 * \param[in]	label	Required, label property of the new container.
 *			Supersedes any label specified in \a cont_prop.
 * \param[in]	cont_prop
 *			Optional, container properties pointer
 *			that if specified must not include an entry
 *			with type DAOS_PROP_CO_LABEL.
 * \param[out]	uuid	Optional, pointer to uuid_t to hold the
 *		        implementation-generated container UUID.
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 *
 * \return		These values will be returned by \a ev::ev_error in
 *			non-blocking mode:
 *			0		Success
 *			-DER_INVAL	Invalid parameter
 *			-DER_NO_HDL	Invalid pool handle
 *			-DER_NO_PERM	Permission denied
 *			-DER_UNREACH	network is unreachable
 */
int
daos_cont_create_with_label(daos_handle_t poh, const char *label,
			    daos_prop_t *cont_prop, uuid_t *uuid,
			    daos_event_t *ev);

/**
 * Open an existing container identified by \a cont, a label or UUID string.
 * Upon successful completion, \a coh and \a info, both of which shall be
 * allocated by the caller, return the container handle and the latest
 * container information respectively.
 *
 * \param[in]	poh	Pool connection handle.
 * \param[in]	cont	Label or UUID string to identify the container.
 * \param[in]	flags	Open flags (DAOS_COO_RO, etc.). Must include one of
 *			DAOS_COO_RO, DAOS_COO_RW, and DAOS_COO_EX.
 * \param[out]	coh	Returned open handle.
 * \param[out]	info	Optional, return container information
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 *
 * \return		These values will be returned by \a ev::ev_error in
 *			non-blocking mode:
 *			0		Success
 *			-DER_INVAL	Invalid parameter
 *			-DER_UNREACH	Network is unreachable
 *			-DER_NO_PERM	Permission denied
 *			-DER_NONEXIST	Container is nonexistent
 *			-DER_RF		Number of failures exceed RF, data possibly lost
 */
int
daos_cont_open(daos_handle_t poh, const char *cont, unsigned int flags, daos_handle_t *coh,
	       daos_cont_info_t *info, daos_event_t *ev);

/**
 * Close a container handle. Upon successful completion, the container handle's
 * epoch hold (i.e., if LHE < DAOS_EPOCH_MAX) is released, and any uncommitted
 * updates from the container handle are discarded.
 *
 * \param[in]	coh	Container open handle.
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 *
 * \return		These values will be returned by \a ev::ev_error in
 *			non-blocking mode:
 *			0		Success
 *			-DER_UNREACH	Network is unreachable
 *			-DER_NO_HDL	Invalid container handle
 */
int
daos_cont_close(daos_handle_t coh, daos_event_t *ev);

/**
 * Destroy a container identified by \a cont, a label or UUID string associated
 * with the container. All objects within this container will be destroyed.
 * If there is at least one container opener, and \a force is set to zero, then
 * the operation completes with DER_BUSY. Otherwise, the container is destroyed
 * when the operation completes.
 *
 * \param[in]	poh	Pool connection handle.
 * \param[in]	cont	Label or UUID string to identify the container to destroy.
 * \param[in]	force	Container destroy will return failure if the container
 *			is still busy (outstanding open handles). This parameter
 *			will force the destroy to proceed even if there is an
 *			outstanding open handle.
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			Function will run in blocking mode if \a ev is NULL.
 *
 * \return		These values will be returned by \a ev::ev_error in
 *			non-blocking mode:
 *			0		Success
 *			-DER_NO_PERM	Permission denied
 *			-DER_UNREACH	Network is unreachable
 *			-DER_NONEXIST	Container is nonexistent
 *			-DER_BUSY	Pool is busy
 */
int
daos_cont_destroy(daos_handle_t poh, const char *cont, int force, daos_event_t *ev);

/**
 * Query container information.
 *
 * \param[in]	coh	Container open handle.
 * \param[out]	info	Returned container information.
 * \param[out]	cont_prop
 *			Optional, returned container properties
 *			If it is NULL, then needs not query the properties.
 *			If cont_prop is non-NULL but its dpp_entries is NULL,
 *			will query all pool properties, DAOS internally
 *			allocates the needed buffers and assign pointer to
 *			dpp_entries.
 *			If cont_prop's dpp_nr > 0 and dpp_entries is non-NULL,
 *			will query the properties for specific dpe_type(s), DAOS
 *			internally allocates the needed buffer for dpe_str or
 *			dpe_val_ptr, if the dpe_type with immediate value then
 *			will directly assign it to dpe_val.
 *			User can free the associated buffer by calling
 *			daos_prop_free().
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 *
 * \return		These values will be returned by \a ev::ev_error in
 *			non-blocking mode:
 *			0		Success
 *			-DER_INVAL	Invalid parameter
 *			-DER_UNREACH	Network is unreachable
 *			-DER_NO_HDL	Invalid container handle
 */
int
daos_cont_query(daos_handle_t coh, daos_cont_info_t *info,
		daos_prop_t *cont_prop, daos_event_t *ev);

/**
 * Query the container Access Control List and ownership properties.
 *
 * \param[in]	coh	Container open handle.

 * \param[out]	acl_prop
 *			Newly allocated daos_prop_t containing the ACL, owner,
 *			and owner-group properties of the container.
 *			Caller must free it with daos_prop_free().
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 *
 * \return		These values will be returned by \a ev::ev_error in
 *			non-blocking mode:
 *			0		Success
 *			-DER_INVAL	Invalid parameter
 *			-DER_UNREACH	Network is unreachable
 *			-DER_NO_HDL	Invalid container handle
 */
int
daos_cont_get_acl(daos_handle_t coh, daos_prop_t **acl_prop, daos_event_t *ev);

/**
 * Sets the container properties.
 *
 * \param[in]	coh	Container handle
 * \param[in]	prop	Property entries to update
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 *
 * \return		These values will be returned by \a ev::ev_error in
 *			non-blocking mode:
 *			0		Success
 *			-DER_INVAL	Invalid parameter
 *			-DER_NO_PERM	Permission denied
 *			-DER_UNREACH	Network is unreachable
 *			-DER_NO_HDL	Invalid container handle
 */
int
daos_cont_set_prop(daos_handle_t coh, daos_prop_t *prop, daos_event_t *ev);

/**
 * Clear container status, to clear container's DAOS_PROP_CO_STATUS property
 * from DAOS_PROP_CO_UNCLEAN status to DAOS_PROP_CO_HEALTHY (with same purpose
 * with "daos cont set-prop --properties=status:healthy --pool= --cont= ".
 *
 * \param[in]	coh	Container handle
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 *
 * \return		These values will be returned by \a ev::ev_error in
 *			non-blocking mode:
 *			0		Success
 *			-DER_UNREACH	Network is unreachable
 *			-DER_NO_HDL	Invalid container handle
 */
int
daos_cont_status_clear(daos_handle_t coh, daos_event_t *ev);

/**
 * Overwrites the container ACL with a new one.
 *
 * \param[in]	coh	Container handle
 * \param[in]	acl	New ACL to write
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 *
 * \return		These values will be returned by \a ev::ev_error in
 *			non-blocking mode:
 *			0		Success
 *			-DER_INVAL	Invalid parameter
 *			-DER_NO_PERM	Permission denied
 *			-DER_UNREACH	Network is unreachable
 *			-DER_NO_HDL	Invalid container handle
 */
int
daos_cont_overwrite_acl(daos_handle_t coh, struct daos_acl *acl,
			daos_event_t *ev);

/**
 * Add new entries and/or update existing entries in a container's ACL.
 *
 * If an entry already exists in the container's ACL for a principal in the
 * passed-in ACL, the entry will be replaced with the new one. Otherwise, a
 * new entry will be added.
 *
 * \param[in]	coh	Container handle
 * \param[in]	acl	ACL containing new/updated entries
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 *
 * \return		These values will be returned by \a ev::ev_error in
 *			non-blocking mode:
 *			0		Success
 *			-DER_INVAL	Invalid parameter
 *			-DER_NO_PERM	Permission denied
 *			-DER_UNREACH	Network is unreachable
 *			-DER_NO_HDL	Invalid container handle
 */
int
daos_cont_update_acl(daos_handle_t coh, struct daos_acl *acl, daos_event_t *ev);

/**
 * Remove a principal's entry from a container's ACL.
 *
 * \param[in]	coh	Container handle
 * \param[in]	type	Principal type to be removed
 * \param[in]	name	Name of principal to be removed (if type is user or
 *			group)
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 *
 * \return		These values will be returned by \a ev::ev_error in
 *			non-blocking mode:
 *			0		Success
 *			-DER_INVAL	Invalid parameter
 *			-DER_NO_PERM	Permission denied
 *			-DER_UNREACH	Network is unreachable
 *			-DER_NO_HDL	Invalid container handle
 *			-DER_NOMEM	Out of memory
 *			-DER_NONEXIST	Principal is not in the ACL
 */
int
daos_cont_delete_acl(daos_handle_t coh, enum daos_acl_principal_type type,
		     d_string_t name, daos_event_t *ev);

/**
 * Update a container's owner user and/or owner group.
 *
 * \param[in]	coh	Container handle
 * \param[in]	user	New owner user (NULL if not updating)
 * \param[in]	group	New owner group (NULL if not updating)
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 *
 * \return		These values will be returned by \a ev::ev_error in
 *			non-blocking mode:
 *			0		Success
 *			-DER_INVAL	Invalid parameter
 *			-DER_NO_PERM	Permission denied
 *			-DER_NONEXIST	User or group does not exist
 *			-DER_UNREACH	Network is unreachable
 *			-DER_NO_HDL	Invalid container handle
 *			-DER_NOMEM	Out of memory
 */
int
daos_cont_set_owner(daos_handle_t coh, d_string_t user, d_string_t group,
		    daos_event_t *ev);

/**
 * Update a container's owner user and/or owner group, without verifying the user/group exists
 * locally on the machine.
 *
 * \param[in]	coh	Container handle
 * \param[in]	user	New owner user (NULL if not updating)
 * \param[in]	group	New owner group (NULL if not updating)
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 *
 * \return		These values will be returned by \a ev::ev_error in
 *			non-blocking mode:
 *			0		Success
 *			-DER_INVAL	Invalid parameter
 *			-DER_NO_PERM	Permission denied
 *			-DER_UNREACH	Network is unreachable
 *			-DER_NO_HDL	Invalid container handle
 *			-DER_NOMEM	Out of memory
 */
int
daos_cont_set_owner_no_check(daos_handle_t coh, d_string_t user, d_string_t group,
			     daos_event_t *ev);

/**
 * List the names of all user-defined container attributes.
 *
 * \param[in]	coh	Container handle.
 * \param[out]	buffer	Buffer containing concatenation of all attribute
 *			names, each being null-terminated. No truncation is
 *			performed and only full names will be returned.
 *			NULL is permitted in which case only the aggregate
 *			size will be retrieved.
 * \param[in,out]
 *		size	[in]: Buffer size. [out]: Aggregate size of all
 *			attribute names (excluding terminating null characters),
 *			regardless of the actual buffer size.
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 */
int
daos_cont_list_attr(daos_handle_t coh, char *buffer, size_t *size,
		    daos_event_t *ev);

/**
 * Retrieve a list of user-defined container attribute values.
 *
 * \param[in]	coh	Container handle
 * \param[in]	n	Number of attributes
 * \param[in]	names	Array of \a n null-terminated attribute names.
 * \param[out]	buffers	Array of \a n buffers to store attribute values.
 *			Attribute values larger than corresponding buffer sizes
 *			will be truncated. NULL values are permitted and will be
 *			treated identical to zero-length buffers, in which case
 *			only the sizes of attribute values will be retrieved.
 * \param[in,out]
 *		sizes	[in]: Array of \a n buffer sizes. [out]: Array of actual
 *			sizes of \a n attribute values, regardless of given
 *			buffer sizes.
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 */
int
daos_cont_get_attr(daos_handle_t coh, int n, char const *const names[],
		   void *const buffers[], size_t sizes[], daos_event_t *ev);

/**
 * Create or update a list of user-defined container attributes.
 *
 * \param[in]	coh	Container handle
 * \param[in]	n	Number of attributes
 * \param[in]	names	Array of \a n null-terminated attribute names.
 * \param[in]	values	Array of \a n attribute values
 * \param[in]	sizes	Array of \a n elements containing the sizes of
 *			respective attribute values.
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 */
int
daos_cont_set_attr(daos_handle_t coh, int n, char const *const names[],
		   void const *const values[], size_t const sizes[],
		   daos_event_t *ev);

/**
 * Delete a list of user-defined container attributes.
 *
 * \param[in]	coh	Container handle
 * \param[in]	n	Number of attributes
 * \param[in]	names	Array of \a n null-terminated attribute names.
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 *
 * \return		These values will be returned by \a ev::ev_error in
 *			non-blocking mode:
 *			0		Success
 *			-DER_INVAL	Invalid parameter
 *			-DER_NO_PERM	Permission denied
 *			-DER_UNREACH	Network is unreachable
 *			-DER_NO_HDL	Invalid container handle
 *			-DER_NOMEM	Out of memory
 */
int
daos_cont_del_attr(daos_handle_t coh, int n, char const *const names[],
		   daos_event_t *ev);

/**
 * Allocate a unique set of 64 bit unsigned integers to be used for object ID
 * generation for that container. This is an optional helper function for
 * applications to use to guarantee unique object IDs on the container when more
 * than 1 client are accessing objects on the container. The highest used ID is
 * tracked in the container metadata for future access to that container. This
 * doesn't guarantee that the IDs allocated are sequential; and several ID
 * ranges could be discarded at container close.
 *
 * \param[in]	coh	Container open handle.
 * \param[in]	num_oids
 *			Number of unique IDs requested.
 * \param[out]	oid	starting oid that was allocated up to oid + num_oids.
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 *
 * \return		These values will be returned by \a ev::ev_error in
 *			non-blocking mode:
 *			0		Success
 *			-DER_NO_HDL	Invalid container open handle
 *			-DER_UNREACH	Network is unreachable
 */
int
daos_cont_alloc_oids(daos_handle_t coh, daos_size_t num_oids, uint64_t *oid,
		     daos_event_t *ev);

/**
 * Trigger aggregation to specified epoch
 *
 * \param[in]	coh	Container handle
 * \param[in]	epoch	Epoch to be aggregated to. Current time will be used
 *			when 0 is specified.
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 */
int
daos_cont_aggregate(daos_handle_t coh, daos_epoch_t epoch, daos_event_t *ev);

/**
 * Rollback to a specific persistent snapshot.
 *
 * \param[in]	coh	Container handle
 * \param[in]	epoch	Epoch of a persistent snapshot to rollback to.
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 */
int
daos_cont_rollback(daos_handle_t coh, daos_epoch_t epoch, daos_event_t *ev);

/**
 * Subscribe to the container snapshot state. If user specifies a valid epoch,
 * the call will return once a persistent snapshot has been taken at that epoch
 * or a greater one. The epoch value will be updated with that epoch. If
 * multiple snapshots exist at an epoch greater than the one specified, the
 * lowest one will be returned in the epoch value. If the epoch value passed in
 * is 0, this call will return the lowest persistent snapshot on the container,
 * if any exist, otherwise will just wait till a persistent snapshot is created.
 *
 * \param[in]	coh	Container handle
 * \param[in,out]
 *		epoch	[in]: Epoch of snapshot to wait for. [out]: epoch of
 *			persistent snapshot that was taken.
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 */
int
daos_cont_subscribe(daos_handle_t coh, daos_epoch_t *epoch, daos_event_t *ev);

/** maximum length of persistent snapshot name */
#define DAOS_SNAPSHOT_MAX_LEN 128

/**
 * Create a persistent snapshot at the current epoch and return it. The epoch
 * that is returned can be used to create a read only transaction to read data
 * from that persistent snapshot. Optionally the snapshot can be given a name as
 * an attribute which can be retrieved with daos_cont_list_snap(). Name length
 * can't exceed DAOS_SNAPSHOT_MAX_LEN.
 *
 * \param[in]	coh	Container handle
 * \param[out]	epoch	returned epoch of persistent snapshot taken.
 * \param[in]	name	Optional null terminated name for snapshot.
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 */
int
daos_cont_create_snap(daos_handle_t coh, daos_epoch_t *epoch, char *name,
		      daos_event_t *ev);

/** snapshot create flag options */
enum daos_snapshot_opts {
	/** create snapshot */
	DAOS_SNAP_OPT_CR	= (1 << 0),
	/** create OI table for a snapshot */
	DAOS_SNAP_OPT_OIT	= (1 << 1),
};

/**
 * Advanced snapshot function, it can do different things based bits set
 * in \a opts:
 * - DAOS_SNAP_OPT_CR
 *   create a snapshot at the current epoch and return it.
 * - DAOS_SNAP_OPT_OIT
 *   create object ID table (OIT) for the snapshot
 *
 * \param[in]	coh	Container handle
 * \param[out]	epoch	returned epoch of persistent snapshot taken.
 * \param[in]	name	Optional null terminated name for snapshot.
 * \param[in]	opts	Bit flags, see daos_snapshot_opts
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 */
int
daos_cont_create_snap_opt(daos_handle_t coh, daos_epoch_t *epoch, char *name,
			  enum daos_snapshot_opts opts, daos_event_t *ev);

/**
 * List all the snapshots of a container and optionally retrieve the snapshot
 * name of each one if it was given at create time.
 *
 * \param[in]	coh	Container handle
 * \param[in,out]
 *		nr	[in]: Number of snapshots in epochs and names.
 *			[out]: Actual number of snapshots returned.
 * \param[out]	epochs	preallocated array of epochs to store snapshots.
 * \param[out]	names	preallocated array of names of the snapshots.
 *			DAOS_SNAPSHOT_MAX_LEN can be used for each name
 *			size if not known.
 * \param[in,out]
 *		anchor	Hash anchor for the next call, it should be set to
 *			zeroes for the first call, it should not be changed
 *			by caller between calls.
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 */
int
daos_cont_list_snap(daos_handle_t coh, int *nr, daos_epoch_t *epochs,
		    char **names, daos_anchor_t *anchor, daos_event_t *ev);

/**
 * Destroy a snapshot. The epoch corresponding to the snapshot is not
 * discarded, but may be aggregated.
 *
 * \param[in]	coh	Container handle
 * \param[in]	epr	Epoch range of snapshots to destroy.
 *			If epr_lo == epr_hi delete 1 snapshot at epr_lo/hi.
 *			If epr_lo == 0, delete all snapshots <= epr_hi.
 *			If epr_hi == DAOS_EPOCH_MAX, delete all snapshots
 *			>= epr_lo.
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 */
int
daos_cont_destroy_snap(daos_handle_t coh, daos_epoch_range_t epr,
		       daos_event_t *ev);

/**
 * Fetch a user's permissions for a specific container.
 *
 * \param[in]	cont_prop	Container property containing DAOS_PROP_CO_ACL/OWNER/OWNER_GROUP
 *				entries
 * \param[in]	uid		User's local uid
 * \param[in]	gids		Gids of the user's groups
 * \param[in]	nr_gids		Length of the gids list
 * \param[out]	perms		Bitmap representing the user's permissions. Bits are defined
 *				in enum daos_acl_perm.
 *
 * \return	0		Success
 *		-DER_INVAL	Invalid input
 *		-DER_NONEXIST	UID or GID not found on the system
 *		-DER_NOMEM	Could not allocate memory
 */
int
daos_cont_get_perms(daos_prop_t *cont_prop, uid_t uid, gid_t *gids, size_t nr_gids,
		    uint64_t *perms);

/**
 * Create object ID table (OIT) for the snapshot
 *
 * \param[in]	coh	Container handle
 * \param[out]	epoch	epoch of snapshot
 * \param[in]	name	Optional null terminated name for snapshot.
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 */
int
daos_cont_snap_oit_create(daos_handle_t coh, daos_epoch_t epoch, char *name,
			  daos_event_t *ev);

/**
 * Destroy object ID table (OIT) for the snapshot
 *
 * \param[in]	coh	Container handle
 * \param[out]	oh	OIT open handle.
 * \param[in]	ev	Completion event, it is optional and can be NULL.
 *			The function will run in blocking mode if \a ev is NULL.
 */
int
daos_cont_snap_oit_destroy(daos_handle_t coh, daos_handle_t oh, daos_event_t *ev);

#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* __DAOS_CONT_H__ */
