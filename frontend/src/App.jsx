import { createEffect, createResource, createSignal, Show } from "solid-js";
import PieceList from "./components/PieceList";
import './styles/App.css'

function App() {
  // User selection of the form: {A=2, B=5, ...}
  const [selection, setSelection] = createSignal({})

  async function getTrackPieces(){
    const response = await fetch('http://localhost:5000/all-pieces')
    const result = await response.json()
    // Transform into array
    return Object.keys(result).map((key)=>{return result[key]})
  }

  const [data] = createResource(getTrackPieces)


  // Print selection when changed
  createEffect(()=>{
    console.log('selection: ')
    console.log(selection())
  })

  return (
    <div>
      <h1>The Brio track generator app!</h1>

      <Show when={!data.loading} fallback={<>Searching...</>}>
        <PieceList pieces={data()} setSelection={setSelection} selection={selection()}/>
        {console.log(data())}
      </Show>

      <button onClick={()=>{console.log(selection())}}>Generate!</button>
    </div>
  );
}

export default App;
