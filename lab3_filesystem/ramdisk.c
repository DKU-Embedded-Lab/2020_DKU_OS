#include <linux/module.h>
#include <linux/blkdev.h>
#include <linux/hdreg.h>
#include <linux/fs.h>
#include <linux/bio.h>
#include <linux/kthread.h>
#include <linux/vmalloc.h>

#ifndef SECTOR_SIZE
#define SECTOR_SIZE		512
#endif
#define DISK_SIZE_IN_BYTES	(512UL * 1024 * 1024)
#define DISK_SIZE_IN_SECTOR	(DISK_SIZE_IN_BYTES / SECTOR_SIZE)// 4 << 21
#define BLK_NAME 		"ramdisk"

struct rd_ext {
	int major;
	struct request_queue *queue;
	struct gendisk *disk;
	void *addr;
	struct task_struct *task;
};

struct rd_ext ext;

struct block_device_operations rd_fops = {
	.owner = THIS_MODULE,
};

int rd_thread_fn(void * data) 
{
	while (!kthread_should_stop()) {
		schedule_timeout(msecs_to_jiffies(1));
	}

	return 0;
}

static void write_copy(struct bio *bio, void *dst)
{
	void *src;
	struct bio_vec bvl;
	struct bvec_iter iter;
	bio_for_each_segment(bvl, bio, iter)
	{
		src = page_address(bvl.bv_page) + bvl.bv_offset;
		memcpy(dst, src, bvl.bv_len);
		dst += bvl.bv_len;
	}
}

static void read_copy(struct bio *bio, void *src)
{
	void *dst;
	struct bio_vec bvl;
	struct bvec_iter iter;
	bio_for_each_segment(bvl, bio, iter)
	{
		dst = page_address(bvl.bv_page) + bvl.bv_offset;
		memcpy(dst, src, bvl.bv_len);
		src += bvl.bv_len;
	}
}

blk_qc_t rd_make_rq(struct request_queue * q, struct bio *bio)
{
	void * addr = ext.addr + (bio->bi_iter.bi_sector << 9);

	if (bio_data_dir(bio) == READ)
		read_copy(bio, addr);
	else 
		write_copy(bio, addr);

	bio_endio(bio);
	return BLK_QC_T_NONE;
}

void * get_disk_space(unsigned long size)
{
	return vmalloc(size);
}

int init_blk_device(void)
{
	int ret = 0;

	ret = register_blkdev(0, BLK_NAME);
	if (ret < 0) {
		printk("Fail to create blk device\n");
		ext.major = 0;
		return -1;
	}

	printk("Major num : %d\n", ret);
	ext.major = ret;
	ext.queue = blk_alloc_queue(GFP_KERNEL);
	blk_queue_make_request(ext.queue, rd_make_rq);
	ext.disk = alloc_disk(16);
	ext.disk->major = ext.major;
	ext.disk->first_minor = 0;
	ext.disk->queue = ext.queue;
	ext.disk->private_data = (void *)&ext;
	ext.disk->fops = &rd_fops;
	sprintf(ext.disk->disk_name, "ramdisk");
	add_disk(ext.disk);
	set_capacity(ext.disk, DISK_SIZE_IN_SECTOR);

	return 0;
}

void cleanup_blk_device(void)
{
	if (ext.disk) {
		del_gendisk(ext.disk);
		put_disk(ext.disk);
		ext.disk = NULL;
	}

	if (ext.major) {
		unregister_blkdev(ext.major, BLK_NAME);
		ext.major = 0;
	}

	return;
}

int __init rd_init(void)
{
	int ret = 0;
	printk("Hello\n");

	ext.addr = get_disk_space(DISK_SIZE_IN_BYTES);
	if (ext.addr)
		printk("success to get disk space\n");
	else {
		printk("fail to get disk space\n");
		ret = -1;
	}

	ret = init_blk_device();
	if (ret < 0) {
		cleanup_blk_device();
	}

	ext.task = kthread_create(rd_thread_fn, NULL, "rd_worker");
	if (IS_ERR(ext.task)) {
		printk("fail to create task\n");
		ret = -1;
	}
	wake_up_process(ext.task);

	return ret;
}

void __exit rd_exit(void)
{
	cleanup_blk_device();
	vfree(ext.addr);
	printk("Bye!\n");
	return;
}

module_init(rd_init);
module_exit(rd_exit);
 
MODULE_LICENSE("GPL");
