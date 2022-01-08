print dir ()
print "dir (CursedEditor):  ", dir (CursedEditor)
print "dir (buffer_vector):  ", dir (buffer_vector)
print "dir (cvar.ed.window):  ", dir (cvar.ed.window)
print "dir (cvar.ed.windows):  ", dir (cvar.ed.windows)

print "cvar.ed: ", cvar.ed
print "cvar.ed.windows[0]: ", cvar.ed.windows[0]
print "cvar.ed.window[0]: ", cvar.ed.window[0]

print "cvar.ed.buffers[0]: ", cvar.ed.buffers[0]
print "cvar.ed.Buffer[0]: ", cvar.ed.Buffer[0]
print "cvar.ed.Buffer: ", cvar.ed.Buffer

print "cvar.ed.window: ", cvar.ed.window

bv = buffer_vector ()
buf = Buffer ()
bv.push_back (buf)

print "bv: ", bv
for i in bv:
  print i
print "bv[0]: ", bv[0]
print "buf.tab_width: ", buf.tab_width
print "bv[0].tab_width: ", bv[0].tab_width

# NB so why does cvar.ed.buffers[0] return a string?
# NB so why does cvar.ed.windows[0] return a string?

print "type (cvar.ed.buffers): ", type (cvar.ed.buffers)
print "type (bv): ", type (bv)

w = cvar.ed.windows
print "type (cvar.ed.windows): ", type (w) # type (cvar.ed.windows)
#print "type (wv): ", type (wv)

ced = CursedEditor ()
print "type (cvar.ed): ", type (cvar.ed)
print "type (ced): ", type (ced)

print "cvar.ed.__dict__['this']: ", cvar.ed.__dict__['this']
ed_r = cvar.ed.__dict__['this']
#print "cursed_editor._swig_getattr (cvar.ed, CursedEditor, 'this'): ", cursed_editor._swig_getattr (cvar.ed, CursedEditor, 'this')
print "type (cvar.ed.__dict__['this']): ", type (cvar.ed.__dict__['this'])

# Buffer an iterator
#   how to get buffer out of iterator?

#set_tab_width (cvar.ed.Buffer, 15)
print "cvar.ed.Buffer.tab_width: ", cvar.ed.Buffer.tab_width

## per buffer!
print "cvar.ed.Buffer.tab_width: ", cvar.ed.Buffer.tab_width

## per window!
print "cvar.ed.window.scroll_margin: ", cvar.ed.window.scroll_margin
cvar.ed.window.scroll_margin = 15
print "cvar.ed.window.scroll_margin: ", cvar.ed.window.scroll_margin


#cvar.ed.Buffer.tab_width = 15
#cvar.ed.window.scroll_margin = 15

#cvar.ed.clear_display ()

cvar.ed.wrap_indicator = ':'
print "cvar.ed.wrap_indicator: ", cvar.ed.wrap_indicator;

print dir (cvar.ed)
print dir (cursed_editor.Editor)
print dir (cursed_editor.CursedEditor)
print dir (cursed_editor.PythonedEditor)

#lllllllllllllllllllllllllllllllllllllllllllllllllllllllloooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooonnnnnnnng
