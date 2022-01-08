global foo

def foo ():
	this.Buffer.forward_chars (1)

foo ()

print c_str ("foo")

#print dir (this)
#print c_str ("foo")
#[this.]bind_key ("s", foo)
bind_key (ord ('s'), foo)
#bind_key (ord ('s'), insert_last_input_char)
#bind_sequence ("s", foo)
transpose_lines ()

#this.Buffer.random_fill (10)
