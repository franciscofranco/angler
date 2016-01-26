/*
 * include/linux/sound_control.h
 *
 * Copyright (c) 2016 Francisco Franco
 * franciscofranco.1990@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef _LINUX_SOUNT_CONTROL_H
#define _LINUX_SOUNT_CONTROL_H

void update_headphones_volume_boost(unsigned int vol_boost);
void update_mic_gain(unsigned int vol_boost);
void update_camera_mic_gain(unsigned int vol_boost);
void update_speakers_l_gain(unsigned int vol_boost);
void update_speakers_r_gain(unsigned int vol_boost);

#endif
