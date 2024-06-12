
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "zbar.h"
#include "image.h"

#pragma pack(1)

typedef struct {
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
} BITMAPINFOHEADER;

int zbar_test(void* raw, int width, int height, char *qrcode, int debug)
{
    int index = 1;
    int ret = -1;
    zbar_image_scanner_t *scanner = NULL;

    /* create a reader */
    scanner = zbar_image_scanner_create();

    /* configure the reader */
    zbar_image_scanner_set_config(scanner, 0, ZBAR_CFG_ENABLE, 1);

    /* wrap image data */
    zbar_image_t *image = zbar_image_create();
    zbar_image_set_format(image, *(int*)"Y800");
    zbar_image_set_size(image, width, height);
    zbar_image_set_data(image, raw, width * height, zbar_image_rt_free_data);

    /* scan the image for barcodes */
    int n = zbar_scan_image(scanner, image);
    if(n == 0) {
        goto exit_entry;
    }
    if (debug) {
        printf("zbar scan image: %d\n", n);
    }

    /* extract results */
    const zbar_symbol_t *symbol = zbar_image_first_symbol(image);
    for(; symbol; symbol = zbar_symbol_next(symbol)) {
        /* do something useful with results */
        zbar_symbol_type_t typ = zbar_symbol_get_type(symbol);
        const char *data = zbar_symbol_get_data(symbol);
        if (debug) {
            printf("decoded %s symbol \"%s\" (len: %d bytes)\r\n", \
                zbar_get_symbol_name(typ), data, (int)strlen(data));
        }

        if (index == 1 && !strcmp(zbar_get_symbol_name(typ), "QR-Code")) {
        	if (qrcode) {
        		memcpy(qrcode, data, strlen(data));
        	}            
            index++;
        }
#if 0
        for(int k=0; k<strlen(data); k++) {
            printf("%X ", (uint8_t)data[k]);
        }
        printf("\n");
#endif
    }

    ret = 0;

exit_entry:
    /* clean up */
    zbar_image_destroy(image);
    zbar_image_scanner_destroy(scanner);

    return ret;
}

int get_qrcode_text(void *raw, int width, int height, char *qrcode, int debug)
{
    uint16_t i,j;
    int qr_img_width = width;
    int qr_img_height = height;   
    uint16_t Color;    
    unsigned char *pic_rgb = (unsigned char *)raw;
    unsigned char *pic_data = NULL;
    unsigned char *pic_hd_data = NULL;

    pic_hd_data = (unsigned char *)malloc(qr_img_width * qr_img_height);
    pic_data = pic_hd_data;

    //printf("pic_hd_data: %p\n", pic_hd_data);
    if(!pic_hd_data) {
        printf("malloc error\n");
        return -1;
    } else {
        //printf("pic_hd_data: %p %p %p %d %d\n", pic_hd_data, raw, pic_rgb, qr_img_width, qr_img_height);
    }
    
    for(i = 0; i < qr_img_width; i++) {
        for(j = 0; j < qr_img_height; j++) {  //将RGB565图片转成灰度
          Color = (*pic_rgb) | (*(pic_rgb + 1)<<8);
          *pic_hd_data = (((Color&0xF800)>> 8)*77+((Color&0x7E0)>>3)*150+((Color&0x001F)<<3)*29)/256;
          pic_hd_data++;
          pic_rgb++;
          pic_rgb++;
        }
    }

    if(zbar_test((void* )pic_data, qr_img_width, qr_img_height, qrcode, debug) < 0) {
        if (debug) {
            printf("zbar failed \n");
        }
    } else {
        if (debug) {
            printf("zbar ok \n");
        }
    }

    rt_free(pic_data);

    return 0;
}

int get_qrcode_text_from_bmp_24bits(const char *bmp_file, char *qrcode)
{
    return 0;
}

#if 1 || ZBAR_MAIN_ENTRY_ENABLE
#include "bmp_test0.h"
#include "bmp_test1.h"
#include "bmp_test2.h"
#include "bmp_test3.h"
#include "bmp_test4.h"
#include "bmp_test5.h"
#include "bmp_test6.h"
#include "bmp_test7.h"

extern const uint8_t gImage_test0[];
extern const uint8_t gImage_test1[];
extern const uint8_t gImage_test2[];
extern const uint8_t gImage_test3[];
extern const uint8_t gImage_test4[];
extern const uint8_t gImage_test5[];
extern const uint8_t gImage_test6[];
extern const uint8_t gImage_test7[];
#endif

#include "bmp_test8.h"
#include "bmp_test9.h"
extern const uint8_t gImage_test8[];
extern const uint8_t gImage_test9[];

int zbar_main(void)
{	  
    char qrcode[64] = {0};
	printf("Hello ZBAR ...\n");

    zbar_qrcode_get_thread_start("online_ok_test");
    zbar_qrcode_get_thread_start("online_fail_test");

/*  
    get_qrcode_text((void* )gImage_test0, 240, 240, qrcode);
    get_qrcode_text((void* )gImage_test1, 460, 460, qrcode);
    get_qrcode_text((void* )gImage_test2, 460, 460, qrcode);
    get_qrcode_text((void* )gImage_test3, 280, 280, qrcode);
    get_qrcode_text((void* )gImage_test4, 460, 460, qrcode);
    get_qrcode_text((void* )gImage_test5, 240, 240, qrcode);
    memset(qrcode, 0, sizeof(qrcode)); 
    get_qrcode_text((void *)gImage_test6, 460, 460, qrcode);
    printf("qrcode: %s\n", qrcode);
    memset(qrcode, 0, sizeof(qrcode));
    get_qrcode_text((void *)gImage_test7, 460, 460, qrcode);
    printf("qrcode: %s\n", qrcode);*/
    //memset(qrcode, 0, sizeof(qrcode));
    //get_qrcode_text_from_bmp_24bits("../qrcode_bmp/wx-qrcode-130608135571250081-24-128x128.bmp", qrcode);
    /*
    get_qrcode_text((void *)gImage_test8, 128, 128, qrcode, 1);
    printf("qrcode: %s\n", qrcode);
    memset(qrcode, 0, sizeof(qrcode));
    get_qrcode_text((void *)gImage_test9, 128, 128, qrcode, 1);
    printf("qrcode: %s\n", qrcode);*/
	
	return 0;
}

#if ZBAR_MAIN_ENTRY_ENABLE

int save_pixel_data(const char *file, const char *title, unsigned char *pixel_data, int size)
{
    int i = 0;
    FILE *out_file = fopen(file, "wb");
    if (!out_file) {
        perror("Error opening input file");
        return 1;
    }

    fprintf(out_file, "const unsigned char %s[] = { /* %d bytes */\n", title, size);
    fprintf(out_file, "    0x%02X, ", pixel_data[0]);
    for (i = 1; i < size; i++) {
        if (i % 16 == 0) {
            fprintf(out_file, "\n    ");
        }
        fprintf(out_file, "0x%02X, ", pixel_data[i]);
    }
    fprintf(out_file, "\n};\n");
    fflush(out_file);
    fclose(out_file);

    return 0;
}

void * test_bmp_file(const char *input_filename, char *qrcode_text, int *size, int get_info_only) 
{
    // 16bits bmp file input
    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file) {
        perror("Error opening input file");
        return NULL;
    }

    BITMAPFILEHEADER header;
    BITMAPINFOHEADER info_header;

    fread(&header, sizeof(BITMAPFILEHEADER), 1, input_file);
    fread(&info_header, sizeof(BITMAPINFOHEADER), 1, input_file);

    printf("%s %d %d %d %d\n", input_filename, info_header.biBitCount, \
            info_header.biWidth, info_header.biHeight, header.bfOffBits);
    unsigned char *pixelData = NULL;
    if (!get_info_only) {
	    fseek(input_file,  header.bfOffBits, SEEK_SET);

	    // 分配像素数据缓冲区
	    int pixel_size = info_header.biWidth * info_header.biHeight * sizeof(unsigned short);
	    pixelData = (unsigned char *)malloc(pixel_size);

	    // 跳过调色板（如果有）
	    printf("%d %d %p\n", header.bfOffBits, pixel_size, pixelData);
	    fseek(input_file, header.bfOffBits, SEEK_SET);
	    fread(pixelData, pixel_size, 1, input_file);

	    get_qrcode_text(pixelData, info_header.biWidth, info_header.biHeight, qrcode_text);
	    printf("qrcode_text: %s\n", qrcode_text);

	    if (size) {
	        *size = pixel_size;
	    }
	}

    fclose(input_file);

    return pixelData;
}

/**
24位深的bmp图片转换为16位深RGB565格式的bmp图片
 **/
struct BGR_data //40B
{
    uint8_t B_data;
    uint8_t G_data;
    uint8_t R_data;
}clr_data[1024*768]; //img size must less than 1024*768 

#define BMP_16BITS_NO_MASK 1

int bmp565_write(unsigned char *image, uint32_t width, uint32_t height, const char *filename)      
{
    unsigned char buffer[1024*1024] = {0};
    uint32_t file_size;     
    uint32_t data_size;     
    unsigned int widthAlignBytes;     
    FILE *fp;     

    // 文件头
#if (BMP_16BITS_NO_MASK)
    unsigned char header[54] =
#else    
    unsigned char header[66] =
#endif
    {     
        // BITMAPFILEINFO     
        'B', 'M',               // [0-1] bfType:必须是BM字符     
        0, 0, 0, 0,             // [2-5] bfSize:总文件大小      
        0, 0, 0, 0,             // [6-9] brReserved1,bfReserved2:保留     
        sizeof(header), 0, 0, 0,// [10-13] bfOffBits:到图像数据的偏移

        // BITMAPFILEHEADER     
        0x28, 0, 0, 0,          // [14-17] biSize:BITMAPINFOHEADER大小40字节     
        0, 0, 0, 0,             // [18-21] biWidth:图片宽度     
        0, 0, 0, 0,             // [22-25] biHeight:图片高度     
        0x01, 0,                // [26-27] biPlanes:必须为1     
        0x10, 0,                // [28-29] biBitCount:16位   
#if (BMP_16BITS_NO_MASK)
        0x00, 0, 0, 0,          // [30-33] biCompression:BI_BITFIELDS=3
#else  
        0x03, 0, 0, 0,          // [30-33] biCompression:BI_BITFIELDS=3
#endif     
        0, 0, 0, 0,             // [34-37] biSizeImage:图片大小     
        0x12, 0x0B, 0, 0,       // [38-41] biXPelsPerMeter:单位长度内的像素数     
        0x12, 0x0B, 0, 0,       // [42-45] biYPelsPerMeter:单位长度内的像素数     
        0, 0, 0, 0,             // [46-49] biClrUsed:可用像素数，设为0即可     
        0, 0, 0, 0,             // [50-53] biClrImportant:重要颜色数，设为0即可     
#if !(BMP_16BITS_NO_MASK)
        // RGBQUAD MASK     
        0x0, 0xF8, 0, 0,        // [54-57] 红色掩码     
        0xE0, 0x07, 0, 0,       // [58-61] 绿色掩码     
        0x1F, 0, 0, 0           // [62-65] 蓝色掩码 
#endif    
    };

    widthAlignBytes = ((width * 16 + 31) & ~31) / 8; // 每行需要的合适字节个数     
    data_size = widthAlignBytes * height;      // 图像数据大小     
    file_size = data_size + sizeof(header);    // 整个文件的大小

    printf("%d data_size:%d, widthAlignBytes:%d, width:%d, height:%d\n",
        sizeof(header), data_size, widthAlignBytes, width, height);

    *((uint32_t*)(header + 2)) = file_size;     
    *((uint32_t*)(header + 18)) = width;     
    *((uint32_t*)(header + 22)) = height;     
    *((uint32_t*)(header + 34)) = data_size;     

    if (!(fp = fopen(filename, "wb")))     
        return -1;     

    fwrite(header, sizeof(unsigned char), sizeof(header), fp);

    if (widthAlignBytes == width * 2)     
    {     
        fwrite(image, sizeof(unsigned char), (size_t)(data_size), fp);
    }     
    else     
    {
        // 每一行单独写入     
        const static int32_t DWZERO = 0;     
        for (int i = 0; i < height; i++)     
        {     
            fwrite(image + i * width * 2, sizeof(unsigned char), (size_t) width * 2, fp);
            fwrite(&DWZERO, sizeof(unsigned char), widthAlignBytes - width * 2, fp);
        }
    }     

    fclose(fp);     
    return 0;     
}

int bmp_24bits_2_16bits(const char *src_file, const char *dst_file)
{
    unsigned char image_data[128*1024] = {0};
    unsigned int image_index = 0;
    FILE *fp,*out_fp;
    int k,q;
    int rec_cout,rec_cout2;
    uint16_t tp_16bit;
    BITMAPFILEHEADER head1;
    BITMAPINFOHEADER head2;

    if((fp=fopen(src_file,"r"))) {
        fseek(fp, 0, 0);//fseek(fp, offset, fromwhere);
        rec_cout=fread(&head1,sizeof(head1),1,fp);

        fseek(fp, 14, 0);//fseek(fp, offset, fromwhere);
        rec_cout2=fread(&head2,sizeof(head2),1,fp);

        fseek(fp, 54, 0);//fseek(fp, offset, fromwhere);
        rec_cout2=fread(&clr_data,3,(head2.biWidth * head2.biHeight),fp);

        fclose(fp);

        printf("write bmp 565 start !\n");
        for(k=0;k<head2.biHeight;k++) {
            for(q=0;q<head2.biWidth;q++) {
                tp_16bit=((uint16_t)(clr_data[k*head2.biWidth+q].R_data>>3)<<11)//R G B
                    +((uint16_t)(clr_data[k*head2.biWidth+q].G_data>>2)<<5)
                    +(uint16_t)(clr_data[k*head2.biWidth+q].B_data>>3);
                image_data[image_index++] = (uint8_t)(tp_16bit&0xff);
                image_data[image_index++] = (uint8_t)(tp_16bit>>8);
            }
        }

        {
            char qrcode[64] = {0};
            get_qrcode_text(image_data, head2.biWidth, head2.biHeight, qrcode);
            printf("qrcode_text: %s\n", qrcode);
        }

        if(bmp565_write(image_data, head2.biWidth, head2.biHeight, dst_file) == 0)
            printf("write bmp 565 success !\n");
        else
            printf("write bmp 565 failed !\n");
    } else {
        printf("error open\n");
    }
    return 0;
}

int main(int argc, const char *argv[])
{    
    char qrcode_text[64] = {0};

    if (argc > 1) {
        if (!strcmp(argv[1], "help")) {

        } else if (!strcmp(argv[1], "gen_header")) {
            int pixel_size = 0;
            const char *bmp = argv[2];
            const char *file = argv[3];
            const char *title = argv[4];
            unsigned char *pixelData = test_bmp_file(bmp, qrcode_text, &pixel_size, 0);
            save_pixel_data(file, title, pixelData, pixel_size);
            free(pixelData);
        } else if (!strcmp(argv[1], "bmp_test")) {
            unsigned char *pixelData = test_bmp_file(argv[2], qrcode_text, NULL, 0);
            free(pixelData);
        } else if (!strcmp(argv[1], "test0")) {
            get_qrcode_text((unsigned char *)gImage_test0, 240, 240, qrcode_text);
            printf("qrcode_text: %s\n", qrcode_text); 
        } else if (!strcmp(argv[1], "test1")) {
            get_qrcode_text((unsigned char *)gImage_test1, 460, 460, qrcode_text);
            printf("qrcode_text: %s\n", qrcode_text); 
        } else if (!strcmp(argv[1], "test2")) {
            get_qrcode_text((unsigned char *)gImage_test2, 280, 280, qrcode_text);
            printf("qrcode_text: %s\n", qrcode_text); 
        } else if (!strcmp(argv[1], "test3")) {
            get_qrcode_text((unsigned char *)gImage_test3, 280, 280, qrcode_text);
            printf("qrcode_text: %s\n", qrcode_text); 
        } else if (!strcmp(argv[1], "xxx")) {
            extern int zbar_main(void);
            zbar_main(); 
        } else if (!strcmp(argv[1], "bmp_info")) {
            test_bmp_file(argv[2], NULL, NULL, 1);
        } else if (!strcmp(argv[1], "bmp_24to16")) {
        	const char *src_file = argv[2];
        	char dst_file[128] = {0};
        	strncpy(dst_file, src_file, strlen(src_file)-4);
        	strcat(dst_file, "-rgb565.bmp");
            bmp_24bits_2_16bits(src_file, dst_file);
        } else {
            printf("error input\n");
        }
    } else {
        printf("error param input\n");
    }

    return 0;
}
#endif
