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
        <div className="piece rounded">
            <img className="piece-img" src={`./src/assets/pieces/${props.piece.id}_resized.png`} alt='Piece image here.'/>
            <section className="rounded piece-characteristics">
                <div>
                    {props.piece.id}
                </div>
                <div>
                    {props.piece.name}
                </div>
                <label>number: </label>
                <input type="number" min="0" className="input-sm" onInput={changeQuantity}/>
            </section>
            
        </div>
    )
}