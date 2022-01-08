/* CursedEditor SWIG interface definition.  */
%module cursed_editor
%{
#include "Editor.h"
#include "PythonedEditor.h"
#include "CursedEditor.h"
%}

%include "std_string.i"
%include "std_vector.i"

namespace std {
    %template(buffer_vector) vector<buffer::Buffer>;
    %template(swindow_vector) vector<SupplyWindow>;
}

//%rename(vector_buffer) vector<buffer::Buffer>;

%include "Cons.i"

%rename(Buffer) buffer;
%include "Buffer.h"

%include "Window.h"
%include "SupplyWindow.h"

%include "Editor.h"
%include "bindables.h"

%include "PythonedEditor.h"
%include "CursedEditor.h"
