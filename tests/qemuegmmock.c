/*
 * Copyright (C) 2024 Red Hat, Inc.
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <config.h>
#include <unistd.h>

#include "internal.h"
#include "virfile.h"
#include "virmock.h"

static bool (*real_virFileExists)(const char *path);
static int (*real_access)(const char *path, int mode);

static void
init_syms(void)
{
    if (real_virFileExists && real_access)
        return;

    VIR_MOCK_REAL_INIT(virFileExists);
    VIR_MOCK_REAL_INIT(access);
}

bool
virFileExists(const char *path)
{
    init_syms();

    /* Mock EGM device paths for testing */
    if (g_str_has_prefix(path, "/dev/egm") ||
        g_str_has_prefix(path, "/sys/class/egm/"))
        return true;

    return real_virFileExists(path);
}

int
access(const char *path, int mode)
{
    init_syms();

    /* Mock EGM device paths for testing */
    if (g_str_has_prefix(path, "/dev/egm") ||
        g_str_has_prefix(path, "/sys/class/egm/"))
        return 0;  /* success */

    return real_access(path, mode);
}
