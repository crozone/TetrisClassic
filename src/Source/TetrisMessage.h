#ifndef _H_TetrisMessage
#define _H_TetrisMessage
#pragma once

// Defines all Tetris game messages

const MessageT	msg_TetrisNewGame				= 1001;
const MessageT	msg_TetrisPauseGame				= 1002;
const MessageT	msg_TetrisResumeGame			= 1003;

const MessageT	msg_TetrisGameTick				= 1010;
const MessageT	msg_TetrisMovePieceLeft			= 1011;
const MessageT	msg_TetrisMovePieceRight		= 1012;
const MessageT	msg_TetrisRotatePieceCCW		= 1013;
const MessageT	msg_TetrisRotatePieceCW			= 1014;
const MessageT	msg_TetrisSoftDropPiece			= 1015;
const MessageT	msg_TetrisHardDropPiece			= 1016;
const MessageT	msg_TetrisHoldPiece				= 1017;

const MessageT msg_TetrisGameStateChanged		= 2000; // CTetrisGame*


#endif // _H_TetrisMessage
