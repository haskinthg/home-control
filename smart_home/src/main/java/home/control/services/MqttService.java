package home.control.services;

import com.fasterxml.jackson.databind.ObjectMapper;
import home.control.model.DeviceDto;
import home.control.repository.DeviceRepository;
import jakarta.annotation.PostConstruct;
import lombok.AllArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.eclipse.paho.client.mqttv3.IMqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.springframework.stereotype.Service;

import java.nio.ByteBuffer;

@Service
@Slf4j
@AllArgsConstructor
public class MqttService {

    private static final int DATA_LENGTH = Long.BYTES;
    private static final ByteBuffer buffer = ByteBuffer.allocate(DATA_LENGTH);

    private final IMqttClient mqttClient;

    private final ObjectMapper objectMapper;

    private final DeviceRepository deviceRepository;

    @PostConstruct
    private void init() {
        String topic = "esp_data";
        try {
            this.subscribe(topic);
            log.info("Подписка на топик {} прошла успешно", topic);
        } catch (MqttException e) {
            log.error("Ошибка подписки на топик", e);
        }
    }

    public void subscribe(final String topic) throws MqttException {
        this.mqttClient.subscribe(topic,  (msg_topic, msg) -> {
            String payload = new String(msg.getPayload());

            DeviceDto deviceDto = this.objectMapper.readValue(payload, DeviceDto.class);
            this.deviceRepository.setDeviceDto(deviceDto);
        });
    }
}
