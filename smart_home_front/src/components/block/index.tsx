import { FC } from "react";
import s from "./style.module.scss";

interface IProps {
  name: string;
  temperature: number;
  humidity: number;
}

export const Block: FC<IProps> = (props) => {
  return (
    <div key={s.id} className={s.block}>
      <div className={s.heading}>{props.name}</div>
      <div className={s.characteristics}>Температура: {props.temperature}</div>
      <div className={s.characteristics}>Влажность: {props.humidity}</div>
    </div>
  );
};