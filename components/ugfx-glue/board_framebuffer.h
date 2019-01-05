/*
 * GDISP framebuffer driver for our eink display, to bridge to ugfx.
 */

/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

#include <sdkconfig.h>

#include <stdint.h>
#include <stdlib.h>

#include <esp_system.h>

#include <badge_pins.h>
#include <badge_nvs.h>
#include <badge_eink.h>
#include <badge_fb.h>
#include <badge_erc12864.h>
#include <badge_disobey_samd.h>

// Set this to your frame buffer pixel format.
#ifndef GDISP_LLD_PIXELFORMAT
	//#ifdef I2C_ERC12864_ADDR
	//	#define GDISP_LLD_PIXELFORMAT		GDISP_PIXELFORMAT_MONO
	//#else
		#define GDISP_LLD_PIXELFORMAT		GDISP_PIXELFORMAT_GRAY256
	//#endif
#endif

// Uncomment this if your frame buffer device requires flushing
#define GDISP_HARDWARE_FLUSH TRUE

// For now we simply keep the current state of the framebuffer in memory, and
// encode it and send it to the badge_eink driver on flush
uint8_t target_lut;

#ifdef I2C_ERC12864_ADDR
#define GDISP_NEED_CONTROL TRUE
#endif

#ifdef GDISP_DRIVER_VMT

	static void board_init(GDisplay *g, fbInfo *fbi) {
#ifdef PIN_NUM_EPD_RESET
		uint8_t eink_type = BADGE_EINK_DEFAULT;
		badge_nvs_get_u8("badge", "eink.dev.type", &eink_type);
		esp_err_t err = badge_eink_init(eink_type);
		assert( err == ESP_OK );

		err = badge_fb_init();
		assert( err == ESP_OK );

		g->g.Width = BADGE_EINK_WIDTH;
		g->g.Height = BADGE_EINK_HEIGHT;
		g->g.Backlight = 100;
		g->g.Contrast = 50;
		fbi->linelen = g->g.Width;
		fbi->pixels = badge_fb;
		target_lut = 2;
#elif defined(I2C_ERC12864_ADDR)
		esp_err_t err = badge_fb_init();
		assert( err == ESP_OK );

		g->g.Width = BADGE_ERC12864_WIDTH;
		g->g.Height = BADGE_ERC12864_HEIGHT;
		g->g.Backlight = 100;
		g->g.Contrast = 50;
		fbi->linelen = g->g.Width;
		fbi->pixels = badge_fb;
#endif
	}

	#if GDISP_HARDWARE_FLUSH
		bool ugfx_screen_flipped = false;
		#ifdef PIN_NUM_EPD_RESET
		static void board_flush(GDisplay *g) {
			(void) g;

			badge_eink_flags_t flags = DISPLAY_FLAG_8BITPIXEL;
			if (ugfx_screen_flipped) {
				flags |= DISPLAY_FLAG_ROTATE_180;
			}

			if (target_lut >= 0xf0)
			{
				// 0xf0 was used in some examples. support it for now..
				badge_eink_display_greyscale(badge_fb, flags, target_lut > 0xf0 ? target_lut - 0xf0 : BADGE_EINK_MAX_LAYERS);
			}
			else if (target_lut > BADGE_EINK_LUT_MAX)
			{
				badge_eink_display(badge_fb, flags);
			}
			else
			{
				badge_eink_display(badge_fb, flags | DISPLAY_FLAG_LUT(target_lut));
			}
		}
		#elif defined(I2C_ERC12864_ADDR)
		static void board_flush(GDisplay *g) {
			printf("ERC12864 FLUSH FROM UGFX!\n");
			badge_erc12864_write_8bitcompat(badge_fb);
		}
		#else
		static void board_flush(GDisplay *g) {
			(void) g;
		}
		#endif
	#endif

	#if GDISP_NEED_CONTROL
		static void board_backlight(GDisplay *g, uint8_t percent) {
			// TODO: Can be an empty function if your hardware doesn't support this
			#ifdef I2C_ERC12864_ADDR
				int value = (percent*255)/100;
				badge_disobey_samd_write_backlight(value);
			#else
				(void) g;
				(void) percent;
			#endif
		}

		static void board_contrast(GDisplay *g, uint8_t percent) {
			// TODO: Can be an empty function if your hardware doesn't support this
			(void) g;
			(void) percent;
		}

		static void board_power(GDisplay *g, powermode_t pwr) {
			// TODO: Can be an empty function if your hardware doesn't support this
			(void) g;
			(void) pwr;
		}
	#endif

#endif /* GDISP_LLD_BOARD_IMPLEMENTATION */
