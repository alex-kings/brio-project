/**
 * Render a single piece.
 */

import PieceCanvas from "./PieceCanvas";

export default function Piece(props){
    return(
        <div className="piece">
            <PieceCanvas parts={props.piece.parts}/>
            <div>
                id: {props.piece.id}
            </div>
            <div>
                name: {props.piece.name}
            </div>
        </div>
    )
}