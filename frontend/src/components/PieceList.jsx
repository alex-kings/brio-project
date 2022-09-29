/**
 * A list of track pieces that the user can select from.
 */

import '../styles/PieceList.css'
import { For, onMount } from "solid-js"
import Piece from './Piece'

export default function PieceList(props){

    return(
        <div className="piece-list">
            <For each={props.pieces}>
                {(piece)=>{
                    return(
                        <Piece piece={piece} setSelection={props.setSelection} selection={props.selection}/>
                    )
                }}
            </For>
        </div>
    )
}

