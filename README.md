# JSON File Parser

Parser for JSON files with functionalities for inspecting and changing the contents of the file.

---

***Terminology used:***

**JSON pair** - a structure consisting of a key that is a string and a value that can be a JSON
object, JSON array, string, keyword or number.

**JSON object** - unordered list of JSON pairs. It can contain zero, one or more JSON pairs.

**JSON array** - ordered list of elements that can be a JSON object, JSON array, string, keyword or number.

**JSON string** - a non-empty string of characters that does not contain any of the characters
*"*, *\n*, *\t*, *<*, *>*.

**JSON keyword** - one of the keywords - *true*, *false* or *null*

**JSON number** - a decimal floating point number

**path** - a chain of keys or indexes, separated by the character */*, that when being followed through 
the corresponding depths of the JSON file point to a specific part of the JSON file


***Example JSON file***

{<br/>
&nbsp;&nbsp;&nbsp;"person": {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"name": "John",<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"age": 25,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"past jobs": [<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"accountant",<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"cashier",<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"zookeeper"<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;],<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"is homeowner": true<br/>
&nbsp;&nbsp;&nbsp;},<br/>
&nbsp;&nbsp;&nbsp;"random": null,<br/>
&nbsp;&nbsp;&nbsp;"colors": [<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;red,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;green,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;blue<br/>
&nbsp;&nbsp;&nbsp;]<br/>
}<br/>


***The program provides the following commands:***

*All parameters should be wrapped with opening **<** character and closing **>** character.*


**1) open &lt;filename&gt;** - Open/load a JSON file. It also executes the validate command.

**2) close &lt;filename&gt;** - Close the currently opened JSON file.
If there are any unsaved changes, a prompt to save is given to the user.

**3) validate &lt;filename&gt;** - Validate a JSON file. If the JSON file doesn't follow the JSON
syntax rules an exception is thrown with information of where and what the error is.


**4) print** - Print the currently opened JSON file in a standard format.

**5) search &lt;key&gt;** - Search for elements in the currently opened JSON file by entering the key
they're under.


**6) traverse &lt;path&gt;** - Traverse the currently opened JSON file by entering a partial or full path. 
The contents after the given path are printed out. From now on the given traversal path is the
starting point and other commands that utilize a path concatenate it to the traversal path.

**7) previous** - Traverse to the previous key or index of the current traversal path
(refer to the traverse command). The contents after the given path are printed out.

**8) go_back** - Clear the current traversal path (refer to the traverse command)
and go to the starting depth of the JSON file. It also executes the print command.


**9) undo** - Undo the most recent change made to the loaded form of the JSON file.

**10) undo_all** - Undo all the changes made to the loaded form of the JSON file.


**11) rename &lt;key&gt; &lt;path&gt;** - Rename the key of a JSON pair in a JSON object by giving the path
to the key.


**12) set_obj &lt;path&gt;** - Set the value of a JSON pair to an empty JSON object by giving the path to the key 
or set the value of an element in a JSON array to an empty JSON object by giving the path to the index.

**13) set_arr &lt;path&gt;** - Set the value of a JSON pair to an empty JSON array by giving the path to the key 
or set the value of an element in a JSON array to an empty JSON array by giving the path to the index.

**14) set_str &lt;string&gt; &lt;path&gt;** - Set the value of a JSON pair to a given string by giving the path to the key 
or set the value of an element in a JSON array to a given string by giving the path to the index.

**15) set_keyword &lt;keyword&gt; &lt;path&gt;** - Set the value of a JSON pair to a given keyword by giving the path to the key 
or set the value of an element in a JSON array to a given keyword by giving the path to the index.

**16) set_num &lt;number> &lt;path&gt;** - Set the value of a JSON pair to a given number by giving the path to the key 
or set the value of an element in a JSON array to a given number by giving the path to the index.

*For example the path **person/name** can be used to set a new name or **"colors/2"** can be used to change
the third color in the JSON array.*


**17) create_obj_in_arr &lt;path&gt;** - Create a new empty JSON object in a JSON array by giving a path to the index
where the new element should be.

**18) create_obj_in_obj &lt;key&gt; &lt;path&gt;** - Create a new JSON pair in a JSON object with a key that is the one that is given
and an empty JSON object as the value by giving a path to the JSON object where the new JSON pair should be in.

**19) create_arr_in_arr &lt;path&gt;** - Create a new empty JSON array in a JSON array by giving a path to the index
where the new element should be.

**20) create_arr_in_obj &lt;key&gt; &lt;path&gt;** - Create a new JSON pair in a JSON object with a key that is the one that is given
and an empty JSON array as the value by giving a path to the JSON object where the new JSON pair should be in.

**21) create_str_in_arr &lt;str&gt; &lt;path&gt;** - Create a new string in a JSON array by giving a path to the index
where the new element should be.

**22) create_str_in_obj &lt;key&gt; &lt;str&gt; &lt;path&gt;** - Create a new JSON pair in a JSON object with a key that is the one that is given
and the given string as the value by giving a path to the JSON object where the new JSON pair should be in.

**23) create_keyword_in_arr &lt;keyword&gt; &lt;path&gt;** - Create a new keyword in a JSON array by giving a path to the index
where the new element should be.

**24) create_keyword_in_obj &lt;key&gt; &lt;keyword&gt; &lt;path&gt;** - Create a new JSON pair in a JSON object with a key that is the one that is given
and the given keyword as the value by giving a path to the JSON object where the new JSON pair should be in.

**25) create_num_in_arr &lt;number&gt; &lt;path&gt;** - Create a new number in a JSON array by giving a path to the index
where the new element should be.

**26) create_num_in_obj &lt;key&gt; &lt;number&gt; &lt;path&gt;** - Create a new JSON pair in a JSON object with a key that is the one that is given
and the given number as the value by giving a path to the JSON object where the new JSON pair should be in.

*Creating in a JSON object is done by giving the path to the JSON object. For example the path **person** can be used to
create a new JSON pair that can assign a different characteristic of the person.
If the user is addressing the starting JSON object, the parameter for the path can be skipped or an empty path can be given.*

*Creating in a JSON array is done by giving the path to the index where the new element should be in the JSON array.
For example the path **person/past jobs/3** can be used to add a new past job to the person in the forth position of the JSON array
(John was fired :( ).*


**27) remove &lt;path&gt;** - Remove a JSON pair in a JSON object by giving the path to its key or 
remove an element in a JSON array by giving the path to its index. 

*For example the path **person/is homeowner** can be used to remove the JSON pair that gives the characteristic whether
the person owns a home or not. The path **"colors/0"** can be used to remove the first color in the JSON array.*


**28) move_from &lt;path&gt;** - Select a JSON pair in a JSON object or an element in a JSON array to be moved
by the move_to_arr or move_to_obj by respectively giving a path to the key of the JSON pair or the index to the element.

**29) move_to_arr &lt;pathTo&gt; &lt;pathFrom&gt;** - Move a JSON pair from a JSON object or an element from a JSON array from the
pathFrom given in the parameter or selected by the move_from command to the pathTo destination that has to end in an index
of a JSON array.

*For example the pathTo **colors/1** and pathFrom **person/past jobs/1** can be used to move the second element from the
JSON array under the key "past jobs" to the second position in the JSON array under the key "colors"*

*Another example are the pathTo **colors/1** and pathFrom **random** can be used to move the value of the JSON pair under
the key "random" to the second position in the JSON array under the key "colors" as an element*


**30) move_to_obj &lt;pathTo&gt; &lt;pathFrom&gt;** - Move a JSON pair from a JSON object or an element from a JSON array from the
pathFrom given in the parameter or selected by the move_from command to the pathTo destination that has to end in the key
of a JSON pair that has a JSON object value or the index of an element in a JSON array that is a JSON object. 

*For example the pathTo **person** and pathFrom **random** can be used to move the JSON pair ("random": null)
to the characteristics in the JSON object under the key "person".*

*Another example are the pathTo **""** and pathFrom **person/past jobs/1** can be used to move an element from the
JSON array under the key "past jobs" to the starting JSON object. The resulting JSON pair has a default key
("Unknown": cashier)*


**31) save &lt;path&gt;** - Save the currently loaded JSON file. A path may be given in order to save only the
elements under it. The traversal path is STILL USED (refer to the traverse command).

*For example not giving a path saves the whole file but giving the path **person** 
will only write the contents under the key "person" to the file.*

**32) save_as &lt;path&gt;** - Save the currently loaded file in another file. A path may be given in order
to save only the elements under it. The traversal path is STILL USED (refer to the traverse command).


**12) help** - Display information about the terminology of the JSON file structure and about the available
commands that the JSON parser supports.

**13) exit** - Clear the loaded data and exit the program.
If there are any unsaved changes, a prompt to save is given to the user.
