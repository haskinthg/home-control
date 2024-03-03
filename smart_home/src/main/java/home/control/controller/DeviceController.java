package home.control.controller;

import home.control.model.DeviceDto;
import home.control.repository.DeviceRepository;
import lombok.AllArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.http.MediaType;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.Collections;
import java.util.List;

@Slf4j
@RestController
@RequestMapping("devices")
@AllArgsConstructor
public class DeviceController {

    private final DeviceRepository deviceRepository;

    @GetMapping(produces = MediaType.APPLICATION_JSON_VALUE)
    public List<DeviceDto> getDevices() {
        List<DeviceDto> deviceDtos = Collections.singletonList(this.deviceRepository.getDeviceDto());
        log.info("devices: {}", deviceDtos);
        return deviceDtos;
    }
}
