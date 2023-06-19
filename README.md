# JSON File Parser

Parser for JSON files with functionalities for changing the content of the file.
It provides the following functions:

**1) open** - Open/load a JSON file.

**2) close** - Close the currently opened JSON file.

**3) validate** - Validate a JSON file. If the JSON file doesn't follow the JSON
syntax rules an exception is thrown with information of where and what the error is.

**4) print** - Print the currently opened JSON file in a well-formatted form.

**5) search** - Search for elements in the currently opened JSON file by entering the key
they're under.

**6) set** - Set a new string value by giving the path to the key in a JSON object or the index
in a JSON array. For example **"members/1/name"** sets a new name or **"colours/2"** changes
the value of the third element in a JSON array.

**7) create** - Create a new JSON pair with a string value in a JSON object
or a new string value in a JSON array.

Creating in a JSON object is done by giving the path to the JSON object. For example
**"members/1"** creates a new JSON pair in the JSON object with information given by the user.
If the user is addressing the starting JSON object a path isn't needed.

Creating in a JSON array is done by giving the path to the new position of the new string
value in the JSON array. For example **"colours/3"** adds a new string value to the forth
position in the JSON array with information given by the user.

**8) remove** - Remove an element by giving the path to the key in a JSON object or the index
in a JSON array. For example **"members/1/name"** removes the JSON pair with the key "name" or
**"colous/2"** removes the third element in the JSON array.

**9) move** - Move a JSON pair from a JSON object or an element from a JSON array to another
place in the file.

Moving TO a JSON object is done by giving the path to move from and the path to move to.
For example **"members/1/birthdate"** and **"offices/0"** moves the JSON pair to the JSON object.
If the user is addressing the starting JSON object a path TO isn't needed.

Moving TO a JSON array is done by giving the path to move from and the path to move to.
For example **"members/1"** and **"offices/2"** moves the element to the third position in the
JSON array.

**10) save** - Save the currently loaded file. A path may be given in order to save only the
elements under it. For example not giving a path saves the whole file but giving the path
**"members/1"** will only write the elements under the second position in the JSON array
to the file.

**11) save_as** - Save the currently loaded file in another file. A path may be given in order
to save only the elements under it much like the save command.

**12) help** - Receive information as to what commands there are and brief information of what
they do.

**13) exit** - Clear up the loaded data and exit the program.
