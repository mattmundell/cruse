## Editing extensions
##
## C-x C-c   to exit.
## C-l       to refresh (eg after output)
## C-x e     to evaluate this file
## After evaluating file:
## M-d       to clip word (might need escape-d instead)
## C-t       to swap chars
## C-x C-t   to swap lines
##

from cursed_editor import is_alphanum, eq, Cons
from cursed_editor import bind_sequence, char_to_pointer

def clip_word ():
	"""Clip next word in current buffer.  Return the word."""

	buff = this.Buffer

	start = buff.point
	has_mark = False

	# Move to the first char in the next word.
	tem = start
	while not eq (tem, None) and False == is_alphanum (tem.car ()):
		if eq (tem, buff.mark):
			has_mark = True
		tem = tem.cdr ()

	end = tem

	# Move to the end of the word.
	while not eq (tem, None) and is_alphanum (tem.car ()):
		if eq (tem, buff.mark):
			has_mark = True
		end = tem
		tem = tem.cdr ()

 	if eq (end, buff.point) and False == is_alphanum (end.car ()):
		return None;

	if eq (end, None):
		cdr = None
	else:
		cdr = end.cdr ()

	# Clip and return the buffer portion from point to the word end.
	if eq (buff.content, buff.point):
		# Portion begins the buffer.
		buff.content = cdr
		buff.point = cdr
		this.window.start = cdr
	else:
		buff.forward_chars (-1)
		buff.point.setcdr (cdr)
		buff.point = cdr
		pass
	if has_mark == True and False == eq (buff.mark, hil):
		# Portion contains the mark
		buff.mark = buff.point
	if False == eq (end, None):
		end.setcdr (None)

	return start

def clip_and_store_word ():
	"""Clip next word, appending or adding it to clips."""
	this.add_clip (clip_word ())
	return 1

def transpose_chars ():
	"""Swap the character at point with the character before point."""
	# In Emacs point is moved forward after swapping, and at the end
	# of a line the previous two characters (i.e. before the newline)
	# are swapped
	buff = this.Buffer
	if buff.content != buff.point:
		tem = buff.point
		buff.forward_chars (-1)
		# swap cars
		car = buff.point.car ()
		buff.point.setcar (tem.car ())
		tem.setcar (car)
		# restore point
		buff.point = tem
	return 1

def transpose_lines ():
	"""Swap the display line at point with the previous one."""
	buff = this.Buffer
	buff.point = buff.beginning_of_line ()
	if False == eq (buff.content, buff.point):
 		if eq (buff.point, None):
			return 1
		two = buff.point
# 		if eq (buff.point, None):
# 			two = Cons (char_to_pointer ('\n'), None)
# 		else:
# 			two = buff.point
		buff.backward_line ()
		one = buff.point
		if eq (buff.point, buff.content):
			# Make two begin the buffer
			buff.point = buff.content = two
			this.window.start = buff.content
		else:
			buff.forward_chars (-1)
			# Link two to the cons preceding one.
			buff.point.setcdr (two)
			buff.forward_chars (1)

		# Move to the end of two.
		while buff.point and False == eq (buff.point.car (), char_to_pointer ('\n')):
			buff.point = buff.point.cdr ()

		three = buff.point.cdr ()
		# Link one to the end of two.
		buff.point.setcdr (one)

		# Move to the end of one.
		buff.forward_chars (1)
		while buff.point and False == eq (buff.point.car (), char_to_pointer ('\n')):
			buff.point = buff.point.cdr ()

		# Link three, the line that followed two, to the end of one.
		buff.point.setcdr (three)
		buff.point = one
	return 1

bind_sequence ("\024", transpose_chars)		 # C-t
bind_sequence ("\030\024", transpose_lines)	 # C-xC-t
bind_sequence ("\033d", clip_and_store_word) # M-d
