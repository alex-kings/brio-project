/**
 * Render a single piece.
 */

export default function Piece(props){

    // Change quantity of this piece selected
    const changeQuantity = (e) => {
        // Important to use spread operator to create hard copy of object.
        const currentSelection = {...props.selection}
        currentSelection[props.piece.id] = e.target.value
        props.setSelection(currentSelection)
    }

    return(
        <tr>
            <td>{props.piece.id}</td>
            <td>{props.piece.name}</td>
            <td><img className="piece-img" src={`/src/assets/pieces/${props.piece.id}_resized.png`} alt='Piece image here.'/></td>
            <td><input type="number" min="0" className="input-me" onInput={changeQuantity}/></td>
        </tr>
    )
}