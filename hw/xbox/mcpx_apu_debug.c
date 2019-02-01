/*
 * QEMU MCPX APU debug helpers
 *
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

#include "mcpx_apu_debug.h"

#include <stddef.h>
#include <stdio.h>
#include "qemu/osdep.h"

#if PLAYBACK_VP_BINS_MASK > 0
# include "audio/audio.h"
#endif

#if PLAYBACK_VP_BINS_MASK > 0
static void vp_bins_out_callback(void *opaque, int free)
{
    printf("VP bins host buffer ran out, %d free bytes\n", free);
}
#endif

#if (PLAYBACK_VP_BINS_MASK > 0) || (DUMP_VP_BINS_MASK > 0)
void debug_vp_bins(MCPXAPUState *d,
                   int32_t mixbin[NUM_MIXBINS][NUM_SAMPLES_PER_FRAME])
{

#if DUMP_VP_BINS_MASK > 0
    MCPX_APU_DPRINTF("mcpx frame ping\n");
    for (int i = 0; i < NUM_MIXBINS; i++) {
        if (d->vp_bin_files[i] == NULL) {
            continue;
        }
        fwrite(&mixbin[i][0], 1, sizeof(mixbin[i]), d->vp_bin_files[i]);
    }
#endif

#if PLAYBACK_VP_BINS_MASK > 0
    /* Output will be S32 mono; mix this now */
    int32_t mixed[NUM_SAMPLES_PER_FRAME] = { 0 };
    for (int i = 0; i < NUM_MIXBINS; i++) {
        if (PLAYBACK_VP_BINS_MASK & (1 << i)) {
            for (int j = 0; j < NUM_SAMPLES_PER_FRAME; j++) {
                mixed[j] += mixbin[i][j] << 8;
            }
        }
    }

    /* Write frame to audio output */
    int n = AUD_write(d->vp_bins_out, mixed, sizeof(mixed));
    if (n < sizeof(mixed)) {
        printf("VP bins host buffer overrun\n");
    }
#endif

}
#endif

#if (PLAYBACK_VP_BINS_MASK > 0) || (DUMP_VP_BINS_MASK > 0)
void initialize_audio_debugger(MCPXAPUState *d)
{

#if PLAYBACK_VP_BINS_MASK > 0
    AUD_register_card("MCPX APU", &d->card);
#endif

#if DUMP_VP_BINS_MASK > 0
    for (unsigned int i = 0; i < NUM_MIXBINS; i++) {
        if (DUMP_VP_BINS_MASK & (1 << i)) {
            char path[32];
            sprintf(path, "vp-bin-%d.bin", i);
            d->vp_bin_files[i] = fopen(path, "wb");
        } else {
            d->vp_bin_files[i] = NULL;
        }
    }
#endif

#if PLAYBACK_VP_BINS_MASK > 0
    struct audsettings vp_bins_as = { 48000, 1, AUD_FMT_S32, 0 };
    d->vp_bins_out = AUD_open_out(&d->card,
                                  d->vp_bins_out,
                                  "mcpx-apu.vp_bins", d,
                                  vp_bins_out_callback,
                                  &vp_bins_as);
//    AUD_set_active_out(d->vp_bins_out, 1); //FIXME: Why does this hang GTK UI?
#endif

}
#endif
