/**
 * Render a single piece.
 */

import { createSignal } from "solid-js"


export default function Piece(props){

    // Change quantity of this piece selected
    const changeQuantity = (e) => {
        // Important to use spread operator to create hard copy of object.
        const currentSelection = {...props.selection}
        currentSelection[props.piece.id] = e.target.value
        props.setSelection(currentSelection)
    }

    return(
        <div className="piece">
            <img className="piece-img" src={`./src/assets/pieces/${props.piece.id}.png`} alt='Piece image here.'/>
            <div>
                id: {props.piece.id}
            </div>
            <div>
                name: {props.piece.name}
            </div>
            <label>number: </label>
            <input type="text" className="input-sm" onInput={changeQuantity}/>
        </div>
    )
}