idf_component_register(SRCS "dht_main.c"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES driver dht11 mqtt esp_wifi nvs_flash esp_event json
)
