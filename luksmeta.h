/* vim: set tabstop=8 shiftwidth=4 softtabstop=4 expandtab smarttab colorcolumn=80: */
/*
 * Copyright (c) 2016 Red Hat, Inc.
 * Author: Nathaniel McCallum <npmccallum@redhat.com>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <libcryptsetup.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t luksmeta_uuid_t[16];

/**
 * Initializes metadata storage on a LUKSv1 device
 *
 * @param cd crypt device handle
 * @return Zero on success or negative errno value otherwise.
 *
 * @note This function returns -EALREADY if a valid header already exists.
 * @note This function returns -ENOSPC if there is insufficient space.
 */
#if defined(__GNUC__)
int __attribute__((nonnull(1), warn_unused_result))
#else
int
#endif
luksmeta_init(struct crypt_device *cd);

/**
 * Gets metadata from the specified slot
 *
 * If buf is NULL or size is smaller than the metadata, this function returns
 * the size of the buffer needed and the uuid.
 *
 * @param cd crypt device handle
 * @param slot requested metadata slot
 * @param uuid the UUID of the metadata (output)
 * @param buf output buffer for metadata (output)
 * @param size size of buf
 * @return The number of bytes in the metadata or negative errno value.
 *
 * @note This function returns -ENOENT if the device has no luksmeta header.
 * @note This function returns -EINVAL if the header or slot data is corrupted.
 * @note This function returns -EBADSLT if the specified slot is invalid.
 * @note This function returns -ENODATA if the specified slot is empty.
 */
#if defined(__GNUC__)
int __attribute__((nonnull(1, 3), warn_unused_result))
#else
int
#endif
luksmeta_get(struct crypt_device *cd, int slot,
             luksmeta_uuid_t uuid, uint8_t *buf, size_t size);

/**
 * Sets metadata to the specified slot
 *
 * The slot parameter may be CRYPT_ANY_SLOT.
 *
 * @param cd crypt device handle
 * @param slot requested metadata slot
 * @param uuid UUID of the metadata
 * @param buf input buffer for metadata
 * @param size size of buf
 * @return The slot number to which data was written or negative errno value.
 *
 * @note This function returns -ENOENT if the device has no luksmeta header.
 * @note This function returns -EINVAL if the header is corrupted.
 * @note This function returns -EBADSLT if the specified slot is invalid.
 * @note This function returns -EKEYREJECTED if the uuid is invalid/reserved.
 * @note This function returns -EALREADY if the specified slot is not empty.
 * @note This function returns -ENOSPC if there is insufficient space.
 */
#if defined(__GNUC__)
int __attribute__((nonnull(1, 3, 4), warn_unused_result))
#else
int
#endif
luksmeta_set(struct crypt_device *cd, int slot,
             const luksmeta_uuid_t uuid, const uint8_t *buf, size_t size);

/**
 * Deletes metadata from the specified slot
 *
 * If uuid is not NULL, this function will confirm that the specified slot
 * has a matching UUID before deletion.
 *
 * @param cd crypt device handle
 * @param slot requested metadata slot
 * @param uuid expected UUID (optional)
 * @return Zero on success or negative errno value otherwise.
 *
 * @note This function returns -ENOENT if the device has no luksmeta header.
 * @note This function returns -EINVAL if the header is corrupted.
 * @note This function returns -EBADSLT if the specified slot is invalid.
 * @note This function returns -EKEYREJECTED if the uuid doesn't match.
 * @note This function returns -EALREADY if the specified slot is empty.
 */
#if defined(__GNUC__)
int __attribute__((nonnull(1)))
#else
int
#endif
luksmeta_del(struct crypt_device *cd, int slot, const luksmeta_uuid_t uuid);

#ifdef __cplusplus
}
#endif
