/* bindables.h -- headers of bindable functions

Functions which can be bound to keys.  Mostly wrappers of class
functionality.  These all use the global ed variable declared in
Editor.h.  Using globals seemed simpler than using pointers to member
function.  */

#ifndef _bindables_h
#define _bindables_h

/* Insert, into the current buffer, the last char read.  Return 1.  */
int insert_last_input_char ();

/* Move to beginning of current line.  Return 1.  */
int bol ();

/* Move to end of current line.  Return 1.  */
int eol ();

/* Set the mark to the current position.  Return 1.  */
int set_mark ();

/* Move forward one character.  Return 1.  */
int forward_char ();

/* Move backward one character.  Return 1.  */
int backward_char ();

/* Clip the next character.  Return 1.  */
int clip_char ();

/* Clip the rest of the current line.  Return 1.  */
int clip_line ();

/* Redraw and center the current window.  Return 1.  */
int center_window ();

/* Insert '\n', the new line character.  Return 1.  */
int insert_newline ();

/* Move forward one line of text.  Return 1.  */
int forward_line ();

/* Move backward one line of text.  Return 1.  */
int backward_line ();

/* Move point forward one page of the display.  Return 1.  */
int forward_window_page ();

/* Move point backward one window page.  Return 1.  */
int backward_window_page ();

/* Clip the region between point and mark.  Return 1.  */
int clip_region ();

/* Write the current buffer to file.  Return 1.  */
int write_buffer ();

/* Return code which breaks out of display loop.  */
int return_break ();

/* Evaluate the current buffer's file.  Return 1.  */
int evaluate_file ();

/* Eval region between point and mark.  Return 1.  */
int evaluate_region ();

/* Swap point and mark.  Return 1.  */
int swap_positions ();

/* Insert the clip at point.  Return 1.  */
int yank ();

/* Clip the previous char.  Return 1.  */
int clip_char_back ();

/* Move to beginning of current buffer.  Return 1.  */
int bob ();

/* Move to end of current buffer.  Return 1.  */
int eob ();

/* Move backward to the first word beginning.  Return 1.  */
int backward_word ();

/* Move forward to the next word end.  Return 1.  */
int forward_word ();

/* Copy the region between point and mark to `clips'.  Return 1.  */
int copy_region ();

/* Fill buffer with 1000 pairs of random floats.  Return 1.  */
int random_fill ();

/* Create a graph from the pairs of floats at point.  Return 1.  */
int graph_pairs ();

/* Evaluate multiple precision int expression on current line.
   Return 1.  */
int eval_mpi_expr ();

/* Load polyhedron from buffer, then echo description of it.
   Return 1.  */
int ppl_buffer ();

#endif /* not _bindables_h */
