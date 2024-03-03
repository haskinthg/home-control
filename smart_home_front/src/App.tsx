import { FC } from "react";
import { Header } from "./components/header";
import './styles.scss';
import { Main } from "./components/main";

export const App: FC = () => {
  return (
    <>
      <Header />
      <Main />
    </>
  );
};