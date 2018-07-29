/**
 * @file
 * @brief Error checking helper utilities for libconfig
 *
 * These definitions provide error checking functionaliy for libconfig
 * function calls. Majority of the API remains intact, so most of the
 * functions have the same naming convention of libconfig including a
 * prepended 'e'. Some functionality has been added such as @c
 * econfig_get_value() and the error and assertion function; the only
 * API change is with @c econfig_setting_get_value() which includes a
 * "path" paramter, thus allowing data retrieval for paths relative to
 * a provided setting.
 *
 * These functions become useful for obtaining configuration
 * information for required settings. If a setting is optional or has
 * other optional features, then the standard libconfig functions
 * should be called, with error checking performed manually for the
 * specific use cases (e.g., a setting path not found may be ok for an
 * optional setting.)
 *
 * @author Jason Graham <jason.graham@nrl.navy.mil>
 * @date 2017-09-19
 */

#ifndef __ECONFIG_H__
#define __ECONFIG_H__

#include <stdlib.h>
#include <stdio.h>
#include <libconfig.h>

#define econfig_lookup_error(file, path)                                                                               \
        {                                                                                                              \
                fprintf(stderr, "error occured in %s: unable to find %s\n", file, path);                               \
                exit(EXIT_FAILURE);                                                                                    \
        }

#define econfig_read_file(config, file)                                                                                \
        {                                                                                                              \
                if (!config_read_file(config, file)) {                                                                 \
                        fprintf(stderr, "error occured in %s:%d\n", config_error_file(config),                         \
                                config_error_line(config));                                                            \
                        exit(EXIT_FAILURE);                                                                            \
                }                                                                                                      \
        }

#define econfig_lookup_assert(file, path, test)                                                                        \
        {                                                                                                              \
                if (!(test))                                                                                           \
                        econfig_lookup_error(file, path);                                                              \
        }

#define econfig_read_error(config)                                                                                     \
        {                                                                                                              \
                fprintf(stderr, "error occured in %s:%d\n", config_error_file(config), config_error_line(config));     \
                exit(EXIT_FAILURE);                                                                                    \
        }

#define econfig_read_assert(config, test)                                                                              \
        {                                                                                                              \
                if (!(test))                                                                                           \
                        econfig_read_error(config);                                                                    \
        }

#define econfig_setting_error(setting)                                                                                 \
        {                                                                                                              \
                fprintf(stderr, "error occured in %s:%d\n", config_setting_source_file(setting),                       \
                        config_setting_source_line(setting));                                                          \
                exit(EXIT_FAILURE);                                                                                    \
        }

#define econfig_setting_assert(setting, test)                                                                          \
        {                                                                                                              \
                if (!(test))                                                                                           \
                        econfig_setting_error(setting);                                                                \
        }

static inline config_setting_t *econfig_lookup(const config_t *config, const char *path)
{
        config_setting_t *s      = config_lookup(config, path);
        config_setting_t *s_file = config_lookup(config, ".");
        assert(s_file); // This should always be non-NULL
        econfig_lookup_assert(config_setting_source_file(s_file), path, s);
        return s;
}

#define econfig_lookup_value(T, config, path, val_addr)                                                                \
        {                                                                                                              \
                if (!config_lookup_##T(config, path, val_addr)) {                                                      \
                        config_setting_t *s_file = econfig_lookup(config, ".");                                        \
                        assert(s_file);                                                                                \
                        econfig_lookup_error(config_setting_source_file(s_file), path);                                \
                }                                                                                                      \
        }

#define econfig_get_value(T, config, path) config_setting_get_##T(econfig_lookup(config, path))

#define econfig_try_get_value(T, config, path)                                                                         \
        (config_lookup(config, path) ? config_setting_get_##T(config_lookup(config, path)) : 0)

static inline config_setting_t *econfig_setting_lookup(config_setting_t *setting, const char *path)
{
        config_setting_t *s = config_setting_lookup(setting, path);
        econfig_lookup_assert(config_setting_source_file(setting), path, s);
        return s;
}

#define econfig_setting_lookup_value(T, setting, name, val_addr)                                                       \
        if (!config_setting_lookup_##T(setting, name, val_addr)) {                                                     \
                econfig_lookup_error(config_setting_source_file(setting), name);                                       \
        }

#define econfig_setting_get_elem_value(T, setting, index)                                                              \
        config_setting_get_##T(econfig_setting_get_elem(setting, index))

static inline config_setting_t *econfig_setting_get_elem(const config_setting_t *setting, unsigned int index)
{
        config_setting_t *s = config_setting_get_elem(setting, index);
        econfig_setting_assert(setting, s);
        return s;
}

#define econfig_setting_get_value(T, setting, path) config_setting_get_##T(econfig_setting_lookup(setting, path))

static inline int econfig_setting_length(const config_setting_t *setting)
{
        int length = config_setting_length(setting);
        econfig_setting_assert(setting, length > 0);
        return length;
}

#endif // __ECONFIG_H__
