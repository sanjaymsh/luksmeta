/* vim: set tabstop=8 shiftwidth=4 softtabstop=4 expandtab smarttab colorcolumn=80: */
/*
 * Copyright (c) 2016 Red Hat, Inc.
 * Author: Nathaniel McCallum <npmccallum@redhat.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "test.h"
#include <errno.h>

static const luksmeta_uuid_t UUID = {
    0xfb, 0x06, 0x8c, 0x1f, 0x68, 0x04, 0x87, 0x9f,
    0xf4, 0xcd, 0x32, 0x25, 0xb2, 0x1a, 0x7e, 0xf8
};

int
main(int argc, char *argv[])
{
    uint8_t data[sizeof(UUID)] = {};
    struct crypt_device *cd = NULL;
    luksmeta_uuid_t uuid = {};
    uint32_t offset = 0;
    uint32_t length = 0;

    /* Test for -ENOENT when there is no luksmeta header. */
    cd = test_format();
    for (int slot = 0; slot < crypt_keyslot_max(CRYPT_LUKS1); slot++) {
        assert(luksmeta_set(cd, slot, UUID, UUID, sizeof(UUID)) == -ENOENT);
        assert(luksmeta_get(cd, slot, uuid, data, sizeof(data)) == -ENOENT);
        assert(luksmeta_del(cd, slot) == -ENOENT);
    }
    crypt_free(cd);

    cd = test_init();
    test_hole(cd, &offset, &length);

    /* Test the layout state. */
    assert(test_layout((range_t[]) {
        { 0, 1024 },                   /* LUKS header */
        { 1024, offset - 1024, true }, /* Keyslot Area */
        { offset, 4096 },              /* luksmeta header */
        END(offset + 4096),            /* Rest of the file */
    }));

    /* Test for -EBADSLT when there is a luksmeta header but no slot. */
    for (int slot = 0; slot < crypt_keyslot_max(CRYPT_LUKS1); slot++) {
        assert(luksmeta_get(cd, slot, uuid, data, sizeof(data)) == -EBADSLT);
        assert(luksmeta_del(cd, slot) == -EBADSLT);
    }

    /* Test for -EALREADY when a valid header is present. */
    assert(luksmeta_init(cd) == -EALREADY);

    crypt_free(cd);
    unlink(filename);
    return 0;
}
