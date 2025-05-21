set(EXTRA_COMPONENT_DIRS ../components/ssd1306)

idf_component_register(SRCS "src/main.c" "src/fm_transmitter.c" "components/ssd1306/ssd1306_i2c_new.c" "components/ssd1306/ssd1306_spi.c" "components/ssd1306/ssd1306.c"
                    INCLUDE_DIRS "." "src" "includes" "components/ssd1306")