package home.control.repository;

import home.control.model.DeviceDto;
import lombok.Getter;
import lombok.Setter;
import org.springframework.stereotype.Service;

@Service
@Getter
@Setter
public class DeviceRepository {
    private DeviceDto deviceDto = new DeviceDto();
}
