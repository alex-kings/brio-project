/**
 * A list of track pieces that the user can select from.
 */

import { For, onMount } from "solid-js"

export default function PieceList(props){

    return(
        <div>
            <For each={props.pieces}>
                {(piece)=>{
                    return(
                        <li>
                            id: {piece.id}
                        </li>
                    )
                }}
            </For>
        </div>
    )
}

