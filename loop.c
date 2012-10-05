/*
 * Copyright (c) 2012 Daniel Mack
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

/*
 * See README
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <alsa/asoundlib.h>

#define BUFSIZE (1024 * 4)

snd_pcm_t *playback_handle, *capture_handle;
int buf[BUFSIZE * 2];

static unsigned int rate = 192000;
static unsigned int format = SND_PCM_FORMAT_S32_LE;

static int open_stream(snd_pcm_t **handle, int dir)
{
	snd_pcm_hw_params_t *hw_params;
	snd_pcm_sw_params_t *sw_params;
	int err;

	if ((err = snd_pcm_open(handle, "default", dir, 0)) < 0) {
		fprintf(stderr, "cannot open audio device (%s)\n", 
			 snd_strerror(err));
		return err;
	}
	   
	if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
		fprintf(stderr, "cannot allocate hardware parameter structure(%s)\n",
			 snd_strerror(err));
		return err;
	}
			 
	if ((err = snd_pcm_hw_params_any(*handle, hw_params)) < 0) {
		fprintf(stderr, "cannot initialize hardware parameter structure(%s)\n",
			 snd_strerror(err));
		return err;
	}

	if ((err = snd_pcm_hw_params_set_access(*handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		fprintf(stderr, "cannot set access type(%s)\n",
			 snd_strerror(err));
		return err;
	}

	if ((err = snd_pcm_hw_params_set_format(*handle, hw_params, format)) < 0) {
		fprintf(stderr, "cannot set sample format(%s)\n",
			 snd_strerror(err));
		return err;
	}

	if ((err = snd_pcm_hw_params_set_rate_near(*handle, hw_params, &rate, NULL)) < 0) {
		fprintf(stderr, "cannot set sample rate(%s)\n",
			 snd_strerror(err));
		return err;
	}

	if ((err = snd_pcm_hw_params_set_channels(*handle, hw_params, 2)) < 0) {
		fprintf(stderr, "cannot set channel count(%s)\n",
			 snd_strerror(err));
		return err;
	}

	if ((err = snd_pcm_hw_params(*handle, hw_params)) < 0) {
		fprintf(stderr, "cannot set parameters(%s)\n",
			 snd_strerror(err));
		return err;
	}

	snd_pcm_hw_params_free(hw_params);

	if ((err = snd_pcm_sw_params_malloc(&sw_params)) < 0) {
		fprintf(stderr, "cannot allocate software parameters structure(%s)\n",
			 snd_strerror(err));
		return err;
	}
	if ((err = snd_pcm_sw_params_current(*handle, sw_params)) < 0) {
		fprintf(stderr, "cannot initialize software parameters structure(%s)\n",
			 snd_strerror(err));
		return err;
	}
	if ((err = snd_pcm_sw_params_set_avail_min(*handle, sw_params, BUFSIZE)) < 0) {
		fprintf(stderr, "cannot set minimum available count(%s)\n",
			 snd_strerror(err));
		return err;
	}
	if ((err = snd_pcm_sw_params_set_start_threshold(*handle, sw_params, 0U)) < 0) {
		fprintf(stderr, "cannot set start mode(%s)\n",
			 snd_strerror(err));
		return err;
	}
	if ((err = snd_pcm_sw_params(*handle, sw_params)) < 0) {
		fprintf(stderr, "cannot set software parameters(%s)\n",
			 snd_strerror(err));
		return err;
	}

	return 0;
}
  
int main(int argc, char *argv[])
{
	int err;

	if ((err = open_stream(&playback_handle, SND_PCM_STREAM_PLAYBACK)) < 0)
		return err;

	if ((err = open_stream(&capture_handle, SND_PCM_STREAM_CAPTURE)) < 0)
		return err;

	if ((err = snd_pcm_prepare(playback_handle)) < 0) {
		fprintf(stderr, "cannot prepare audio interface for use(%s)\n",
			 snd_strerror(err));
		return err;
	}
	
	if ((err = snd_pcm_start(capture_handle)) < 0) {
		fprintf(stderr, "cannot prepare audio interface for use(%s)\n",
			 snd_strerror(err));
		return err;
	}

	memset(buf, 0, sizeof(buf));

	while (1) {
		int avail;

		if ((err = snd_pcm_wait(playback_handle, 1000)) < 0) {
			fprintf(stderr, "poll failed(%s)\n", strerror(errno));
			break;
		}	           

		avail = snd_pcm_avail_update(capture_handle);
		if (avail > 0) {
			if (avail > BUFSIZE)
				avail = BUFSIZE;

			snd_pcm_readi(capture_handle, buf, avail);
		}

		avail = snd_pcm_avail_update(playback_handle);
		if (avail > 0) {
			if (avail > BUFSIZE)
				avail = BUFSIZE;

			snd_pcm_writei(playback_handle, buf, avail);
		}
	}

	snd_pcm_close(playback_handle);
	snd_pcm_close(capture_handle);
	return 0;
}
