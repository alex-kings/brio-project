import PieceList from "./components/PieceList";

function App() {

  async function getTrackPieces(){
    const response = await fetch('http://localhost:5000/all-pieces')
    const result = await response.json()
    // Transform into array
    return Object.keys(result).map((key)=>{return result[key]})
  }


  return (
    <div>
      <h1>hello</h1>
      <PieceList/>
    </div>
  );
}

export default App;
