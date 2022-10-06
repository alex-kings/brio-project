import MainPage from "./components/MainPage";
import Welcome from "./components/Welcome";
import './styles/App.css'

export default function App() {
  return(
    <div className="app">
      <Welcome/>
      <MainPage/>
    </div>
  )
}

