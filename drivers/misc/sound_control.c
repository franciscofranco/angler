/*
 * Copyright 2015-2016 Francisco Franco
 * franciscofranco.1990@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/sound_control.h>

#define MAX_VALUE 20

/*
 * Heaphones
 */
unsigned int headphones_boost = 0;

/*
 * Mic
 */
unsigned int mic_boost = 0;

/*
 * Camera mic
 */
unsigned int camera_mic_boost = 0;

/*
 * Speakers
 */
unsigned int speaker_l_boost = 0;
unsigned int speaker_r_boost = 0;

/*
 * Sysfs get/set entries
 */

static ssize_t headphones_boost_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", headphones_boost);
}

static ssize_t headphones_boost_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int ret;
	unsigned long val;

	ret = kstrtoul(buf, 0, &val);
	if (ret < 0)
		return ret;

	headphones_boost = val > MAX_VALUE ? MAX_VALUE : val;

	update_headphones_volume_boost(headphones_boost);

	pr_info("%s: %d\n", __func__, headphones_boost);
	return size;
}

static ssize_t mic_boost_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", mic_boost);
}

static ssize_t mic_boost_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int ret;
        unsigned long val;

        ret = kstrtoul(buf, 0, &val);
        if (ret < 0)
                return ret;

        mic_boost = val > MAX_VALUE ? MAX_VALUE : val;

        update_mic_gain(mic_boost);

        pr_info("%s: %d\n", __func__, mic_boost);
        return size;
}

static ssize_t camera_mic_boost_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", camera_mic_boost);
}

static ssize_t camera_mic_boost_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int ret;
        unsigned long val;

        ret = kstrtoul(buf, 0, &val);
        if (ret < 0)
                return ret;

        camera_mic_boost = val > MAX_VALUE ? MAX_VALUE : val;

        update_camera_mic_gain(camera_mic_boost);

        pr_info("%s: %d\n", __func__, camera_mic_boost);
        return size;
}

static ssize_t speaker_l_boost_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", speaker_l_boost);
}

static ssize_t speaker_l_boost_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int ret;
        unsigned long val;

        ret = kstrtoul(buf, 0, &val);
        if (ret < 0)
                return ret;

        speaker_l_boost = val > MAX_VALUE ? MAX_VALUE : val;

        update_speakers_l_gain(speaker_l_boost);

        pr_info("%s: %d\n", __func__, speaker_l_boost);
        return size;
}

static ssize_t speaker_r_boost_show(struct device *dev,
                struct device_attribute *attr, char *buf)
{
        return sprintf(buf, "%d\n", speaker_r_boost);
}

static ssize_t speaker_r_boost_store(struct device *dev,
                struct device_attribute *attr, const char *buf, size_t size)
{
	int ret;
        unsigned long val;

        ret = kstrtoul(buf, 0, &val);
        if (ret < 0)
                return ret;

        speaker_r_boost = val > MAX_VALUE ? MAX_VALUE : val;

        update_speakers_r_gain(speaker_r_boost);

        pr_info("%s: %d\n", __func__, speaker_r_boost);
        return size;
}

static DEVICE_ATTR(volume_boost, 0664, headphones_boost_show,
	headphones_boost_store);
static DEVICE_ATTR(mic_boost, 0664, mic_boost_show, mic_boost_store);
static DEVICE_ATTR(camera_mic_boost, 0664, camera_mic_boost_show, camera_mic_boost_store);
static DEVICE_ATTR(speaker_l_boost, 0664, speaker_l_boost_show, speaker_l_boost_store);
static DEVICE_ATTR(speaker_r_boost, 0664, speaker_r_boost_show, speaker_r_boost_store);

static struct attribute *soundcontrol_attributes[] =
{
	&dev_attr_volume_boost.attr,
	&dev_attr_mic_boost.attr,
	&dev_attr_camera_mic_boost.attr,
	&dev_attr_speaker_l_boost.attr,
	&dev_attr_speaker_r_boost.attr,
	NULL
};

static struct attribute_group soundcontrol_group =
{
	.attrs  = soundcontrol_attributes,
};

static struct miscdevice soundcontrol_device =
{
	.minor = MISC_DYNAMIC_MINOR,
	.name = "soundcontrol",
};

static void __exit soundcontrol_exit(void)
{
	misc_deregister(&soundcontrol_device);
}

static int __init soundcontrol_init(void)
{
	int ret;

	pr_info("%s misc_register(%s)\n", __func__,
		soundcontrol_device.name);

	ret = misc_register(&soundcontrol_device);

	if (ret) {
		pr_err("%s misc_register(%s) fail\n", __func__,
			soundcontrol_device.name);
		return -EINVAL;
	}

	if (sysfs_create_group(&soundcontrol_device.this_device->kobj,
			&soundcontrol_group) < 0) {
		pr_err("%s sysfs_create_group fail\n", __func__);
		pr_err("Failed to create sysfs group for device (%s)!\n",
			soundcontrol_device.name);
	}

	return 0;
}
late_initcall(soundcontrol_init);
module_exit(soundcontrol_exit);
