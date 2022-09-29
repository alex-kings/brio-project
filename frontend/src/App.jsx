import { createResource, Show } from "solid-js";
import PieceList from "./components/PieceList";

function App() {

  async function getTrackPieces(){
    const response = await fetch('http://localhost:5000/all-pieces')
    const result = await response.json()
    // Transform into array
    return Object.keys(result).map((key)=>{return result[key]})
  }

  const [data] = createResource(getTrackPieces)

  return (
    <div>
      <h1>hello</h1>

      <Show when={!data.loading} fallback={<>Searching...</>}>
        <PieceList pieces={data()}/>
        {console.log(data())}
      </Show>
    </div>
  );
}

export default App;
