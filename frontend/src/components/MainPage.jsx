/**
 * Main page.
 */

import { createEffect, createSignal, onMount, Show } from "solid-js";
import PieceList from "./PieceList";
import TrackCanvas from "./TrackCanvas";
import "../styles/TrackCanvas.css"

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

        const resp = await fetch("http://localhost:5000/generate_track", {
            method: "POST",
            headers: {'Content-Type': 'application/json'}, 
            body: JSON.stringify(selection())
        }).catch(e => {console.log(e)})
        const result = await resp.json()
        console.log("Track generated: ", result)
    }

    const hasPieces = () => {
        return Object.keys(pieces()).length !== 0
    }

    return (
        <div className="two-cols">
            <div>
                <div className="scrollable">
                    <PieceList pieces={pieces()} setSelection={setSelection} selection={selection()} />
                </div>
                <button className="btn" onClick={generateTrack}>Generate</button>
            </div>
            <div className="canvas-container">
                <h3>track</h3>
                <Show when={hasPieces()} fallback="Loading...">
                    <TrackCanvas pieces={pieces()}/>
                </Show>
            </div>
        </div>
    )
}