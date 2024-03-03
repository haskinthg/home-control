import {FC, useEffect, useState} from "react";
import axios from 'axios';
import s from "./styles.module.scss";
import {Device} from "../../model/model";
import {Block} from "../block";

export const Main: FC = () => {
    const [devices, setDevices] = useState<Device[]>([]);

    useEffect(() => {
        getData();
        const intervalId = setInterval(getData, 10000);
        return () => clearInterval(intervalId);
    }, []);

    function getData() {
        axios.get<Device[]>("/api/devices")
            .then(responce => {
                setDevices(responce.data);
            });
    }

    const devicesArray = Object.values(devices);

    return (
        <div className={s.main}>
            {devicesArray.map((dev) => (
                <Block
                    key={dev.id}
                    name={dev.deviceName}
                    temperature={dev.temperature}
                    humidity={dev.humidity}
                />
            ))}
        </div>
    );
};