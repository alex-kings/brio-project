/**
 * Main page.
 */

import { createEffect, createSignal, onMount } from "solid-js";
import PieceList from "./PieceList";
import TrackCanvas from "./TrackCanvas";
import "../styles/Canvas.css"

export default function MainPage() {

    // User selection of the form: {A="2", B="5", ...}
    const [selection, setSelection] = createSignal({})
    const [pieces, setPieces] = createSignal({})

    // Fetch pieces
    onMount(async () => {
        const response = await fetch('http://localhost:5000/all_pieces')
        const result = await response.json()

        setPieces(Object.keys(result).map((key) => { return result[key] }))
    })

    // Sends pieces to backend and gets generated track
    const generateTrack = async () => {
        console.log(selection())

        const resp = await fetch("http://localhost:5000/send_piece_selection", {
            method: "POST",
            headers: {'Content-Type': 'application/json'}, 
            body: JSON.stringify(selection())
        }).catch(e => {console.log(e)})
        const result = await resp.json()

    }

    // Display track only when pieces are available
    function displayTrack() {
        if(!(Object.keys(pieces()).length === 0)) {
            return <TrackCanvas pieces={pieces()}/>
        }
        return <></>
    }

    return (
        <div className="two-cols">
            <div>
                <PieceList pieces={pieces()} setSelection={setSelection} selection={selection()} />
                <button className="btn" onClick={generateTrack()}>Generate</button>
            </div>
            <div className="canvas-container">
                <h3>track</h3>
                {displayTrack()}
            </div>
        </div>
    )
}