## Python extension examples
##
## C-x C-c   to exit.
## C-l       to refresh (eg after output)
## C-space   to set mark (use to create region)
## C-x C-e   to evaluate region (runs the python code in the region!)
## C-x C-s   to save file
##

print "Hello, world."

"Any printing is echoed."

print "dir (): ", dir ()

## Change the wrap indicator
this.wrap_indicator = ':'

## Move forward 30 characters and insert an 'A'
this.Buffer.forward_chars (30)
this.Buffer.insert_char (char_to_pointer ('A'))

## Change the scroll margin
this.window.scroll_margin = 15

## Test the change
# loooooooAoooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooonnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnng line

## Import and use a module
import sys
print "sys.stdout.isatty(): ", sys.stdout.isatty()


# Point (cursor) is coloured red and mark a greeny yellow.
# At the time of the screen shot mark was here:  and
# point here:


print "Goodbye."


## Errors are included in the echo area
that.wrap_indicator = '/'  # "NameError: name 'that' is..."
