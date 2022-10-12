/**
 * A list of track pieces that the user can select from.
 */

import '../styles/PieceList.css'
import { For, onMount } from "solid-js"
import Piece from './Piece'

export default function PieceList(props) {

    return (
        <table className="table">
            <tbody>
                <tr>
                    <th>ID</th>
                    <th>Description</th>
                    <th>Image</th>
                    <th>Selection</th>
                </tr>
                <For each={props.pieces} fallback={<p>Loading...</p>}>
                    {(piece) => (

                        <Piece piece={piece} setSelection={props.setSelection} selection={props.selection} />

                    )}
                </For>

            </tbody>
        </table>
    )
}

