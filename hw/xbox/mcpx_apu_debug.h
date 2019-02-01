/*
 * QEMU MCPX APU debug helpers
 *
 * Copyright (c) 2012 espes
 * Copyright (c) 2019 Jannik Vogel
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HW_MCPX_APU_DEBUG_H
#define HW_MCPX_APU_DEBUG_H

// #define DEBUG_MCPX_APU
#ifdef DEBUG_MCPX_APU
# define MCPX_APU_DPRINTF(format, ...)       printf("mcpx-apu: " format, ## __VA_ARGS__)
#else
# define MCPX_APU_DPRINTF(format, ...)       do { } while (0)
#endif

/* Generate a tone in the MIXBIN for every channel */
#define GENERATE_MIXBIN_BEEP      0

/* Debug feature to enable VP audio output for debugging */
#define PLAYBACK_VP_BINS_MASK     0x00000001 /* 32-bits, 1 bit per bin */
#define DUMP_VP_BINS_MASK         0x00000001 /* 32-bits, 1 bit per bin */

#endif
