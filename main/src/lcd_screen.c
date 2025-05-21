
#include "lcd_screen.h"

void lcd_demo(void) {

    int center, top, bottom;
	char lineChar[20];


	ssd1306_clear_screen(&lcd_dev, false);
	ssd1306_contrast(&lcd_dev, 0xff);
	ssd1306_display_text_x3(&lcd_dev, 0, "Hello", 5, false);
	vTaskDelay(3000 / portTICK_PERIOD_MS);

	top = 2;
	center = 3;
	bottom = 8;
	ssd1306_display_text(&lcd_dev, 0, "SSD1306 128x64", 14, false);
	ssd1306_display_text(&lcd_dev, 1, "ABCDEFGHIJKLMNOP", 16, false);
	ssd1306_display_text(&lcd_dev, 2, "abcdefghijklmnop",16, false);
	ssd1306_display_text(&lcd_dev, 3, "Hello World!!", 13, false);
	//ssd1306_clear_line(&lcd_dev, 4, true);
	//ssd1306_clear_line(&lcd_dev, 5, true);
	//ssd1306_clear_line(&lcd_dev, 6, true);
	//ssd1306_clear_line(&lcd_dev, 7, true);
	ssd1306_display_text(&lcd_dev, 4, "SSD1306 128x64", 14, true);
	ssd1306_display_text(&lcd_dev, 5, "ABCDEFGHIJKLMNOP", 16, true);
	ssd1306_display_text(&lcd_dev, 6, "abcdefghijklmnop",16, true);
	ssd1306_display_text(&lcd_dev, 7, "Hello World!!", 13, true);

	vTaskDelay(3000 / portTICK_PERIOD_MS);
	
	// Display Count Down
	uint8_t image[24];
	memset(image, 0, sizeof(image));
	ssd1306_display_image(&lcd_dev, top, (6*8-1), image, sizeof(image));
	ssd1306_display_image(&lcd_dev, top+1, (6*8-1), image, sizeof(image));
	ssd1306_display_image(&lcd_dev, top+2, (6*8-1), image, sizeof(image));
	for(int font=0x39;font>0x30;font--) {
		memset(image, 0, sizeof(image));
		ssd1306_display_image(&lcd_dev, top+1, (7*8-1), image, 8);
		memcpy(image, font8x8_basic_tr[font], 8);
		if (lcd_dev._flip) ssd1306_flip(image, 8);
		ssd1306_display_image(&lcd_dev, top+1, (7*8-1), image, 8);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
	
	// Scroll Up
	ssd1306_clear_screen(&lcd_dev, false);
	ssd1306_contrast(&lcd_dev, 0xff);
	ssd1306_display_text(&lcd_dev, 0, "---Scroll  UP---", 16, true);
	//ssd1306_software_scroll(&lcd_dev, 7, 1);
	ssd1306_software_scroll(&lcd_dev, (lcd_dev._pages - 1), 1);
	for (int line=0;line<bottom+10;line++) {
		lineChar[0] = 0x01;
		sprintf(&lineChar[1], " Line %02d", line);
		ssd1306_scroll_text(&lcd_dev, lineChar, strlen(lineChar), false);
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
	vTaskDelay(3000 / portTICK_PERIOD_MS);
	
	// Scroll Down
	ssd1306_clear_screen(&lcd_dev, false);
	ssd1306_contrast(&lcd_dev, 0xff);
	ssd1306_display_text(&lcd_dev, 0, "--Scroll  DOWN--", 16, true);
	//ssd1306_software_scroll(&lcd_dev, 1, 7);
	ssd1306_software_scroll(&lcd_dev, 1, (lcd_dev._pages - 1) );
	for (int line=0;line<bottom+10;line++) {
		lineChar[0] = 0x02;
		sprintf(&lineChar[1], " Line %02d", line);
		ssd1306_scroll_text(&lcd_dev, lineChar, strlen(lineChar), false);
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
	vTaskDelay(3000 / portTICK_PERIOD_MS);

	// Page Down
	ssd1306_clear_screen(&lcd_dev, false);
	ssd1306_contrast(&lcd_dev, 0xff);
	ssd1306_display_text(&lcd_dev, 0, "---Page	DOWN---", 16, true);
	ssd1306_software_scroll(&lcd_dev, 1, (lcd_dev._pages-1) );
	for (int line=0;line<bottom+10;line++) {
		//if ( (line % 7) == 0) ssd1306_scroll_clear(&lcd_dev);
		if ( (line % (lcd_dev._pages-1)) == 0) ssd1306_scroll_clear(&lcd_dev);
		lineChar[0] = 0x02;
		sprintf(&lineChar[1], " Line %02d", line);
		ssd1306_scroll_text(&lcd_dev, lineChar, strlen(lineChar), false);
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
	vTaskDelay(3000 / portTICK_PERIOD_MS);

	// Horizontal Scroll
	ssd1306_clear_screen(&lcd_dev, false);
	ssd1306_contrast(&lcd_dev, 0xff);
	ssd1306_display_text(&lcd_dev, center, "Horizontal", 10, false);
	ssd1306_hardware_scroll(&lcd_dev, SCROLL_RIGHT);
	vTaskDelay(5000 / portTICK_PERIOD_MS);
	ssd1306_hardware_scroll(&lcd_dev, SCROLL_LEFT);
	vTaskDelay(5000 / portTICK_PERIOD_MS);
	ssd1306_hardware_scroll(&lcd_dev, SCROLL_STOP);
	
	// Vertical Scroll
	ssd1306_clear_screen(&lcd_dev, false);
	ssd1306_contrast(&lcd_dev, 0xff);
	ssd1306_display_text(&lcd_dev, center, "Vertical", 8, false);
	ssd1306_hardware_scroll(&lcd_dev, SCROLL_DOWN);
	vTaskDelay(5000 / portTICK_PERIOD_MS);
	ssd1306_hardware_scroll(&lcd_dev, SCROLL_UP);
	vTaskDelay(5000 / portTICK_PERIOD_MS);
	ssd1306_hardware_scroll(&lcd_dev, SCROLL_STOP);
	
	// Invert
	ssd1306_clear_screen(&lcd_dev, true);
	ssd1306_contrast(&lcd_dev, 0xff);
	ssd1306_display_text(&lcd_dev, center, "  Good Bye!!", 12, true);
	vTaskDelay(5000 / portTICK_PERIOD_MS);


	// Fade Out
	ssd1306_fadeout(&lcd_dev);

	// Restart module
	esp_restart();

    return;
}


