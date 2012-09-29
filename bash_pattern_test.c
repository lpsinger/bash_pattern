/*
 * Copyright (C) 2012  Leo Singer <leo.singer@ligo.org>
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


#include <bash_pattern.h>
#include <stdio.h>
#include <sysexits.h>

int main(int argc, char **argv)
{
    int ret;
    bash_pattern *pattern;

    if (argc < 3)
    {
        puts("Usage: bash_pattern_test 'pattern' 'text'");
        return EX_USAGE;
    }

    fprintf(stderr, "Compiling pattern: %s\n", argv[1]);
    pattern = bash_pattern_compile(argv[1]);

    fprintf(stderr, "Matching against text: %s\n", argv[2]);
    ret = bash_pattern_matches(pattern, argv[2]);

    if (ret)
        fprintf(stderr, "Match: YES\n");
    else
        fprintf(stderr, "Match: NO\n");

    fprintf(stderr, "Freeing pattern\n");
    bash_pattern_free(pattern);

    return !ret;
}
