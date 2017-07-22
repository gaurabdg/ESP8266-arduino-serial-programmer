
var filename = null;
var fileData = null;
var host = location.host;
var espURL = "http://" + host;

function UploadData()
{
	var xhr = new XMLHttpRequest();
	var url = espUrl + "/upload&"+ filename
	var params = fileData;
	xhr.open("POST", url, true);
	xhr.send(params);
}

function UploadFile() 
{
	UploadData();
	setTimeout(fetchFileList, 2000);
}

var caution = 0;
function readSingleFile(e) // since in first run above does nothing, and after we upload a file, click is recorded, this function is set in motion.
{
	var file = e.target.files[0]; // return the file taht waws uploaded
	filename = file.name;	
	if (!file) 
		{ 
			caution = 1; 
		}
	var reader = new FileReader();
	reader.onload = function(e) 
	{
		fileData = e.target.result;		
	};
	reader.readAsText(file);
	
	
}

function fetchFileList() 
{
	var xhr = new XMLHttpRequest();
	var url = espUrl + "/files";

	xhr.open("GET", url, true);
	xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	xhr.onreadystatechange = function() 
	{
		if(xhr.readyState == 4 && xhr.status == 200) //done & success
		{
			DisplayDirectory(xhr.responseText);
			alert("rcv");
   		} 
   		else 
   		{
			alert("Failed to connect");
		}
	}
	
	xhr.send();
}

function deleteFile(filename) 
{
	var xhr = new XMLHttpRequest();
	var url = espUrl + "/delete&" + filename;

	xhr.open("GET", url, true);
	xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	xhr.onreadystatechange = function() 
	{
		if(xhr.readyState == 4 && xhr.status == 200) 
		{
			DisplayDirectory(xhr.responseText);
   		} 
   		else 
   		{
			alert("Failed to connect");
		}
	}
	xhr.send();
}

function flashFile(filename) 
{
	var xhr = new XMLHttpRequest();
	var url = espUrl + "/flash&" + filename;

	xhr.open("GET", url, true);
	xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	xhr.onreadystatechange = function() 
	{
		if(xhr.readyState == 4 && xhr.status == 200) 
		{
			DisplayDirectory(xhr.responseText);
   		}
	}
	xhr.send();
}



var checkRow = 0;
function GenerateRows(filename, filesize)
{
	var table = document.getElementById("table");
    if(checkRow == 0)
    { 
    	document.getElementById("table").deleteRow(0);
    }
    var row = table.insertRow(checkRow);
    checkRow = checkRow +1;
    var cell1 = row.insertCell(0);
    var cell2 = row.insertCell(1);
    cell1.innerHTML = filename;
    cell2.innerHTML = filesize;
}


function DisplayDirectory(requestList)
{
	var listData = requestList.split("\n");
	if(listData.length == 0)
	{
		checkRow = 0;
	}

	var totalsize = 0;
	var filecount = 0;

	for(var i = 0; i < listData.length; i++)
	{
		if(listData.length < 5)
		{
			continue;
		}
		filecount++;
		var elements = listData.split(";");
		totalsize += parseInt(elements[1]);
		GenerateRows(elements[0], elements[1]);
	}
	var lastRow = filecount - 1;
	var table1 = document.getElementById("tab");
	var rowTotal = table1.insertRow(lastRow);
	var cell3 = rowTotal.insertCell(0);
	var cell4 = rowTotal.insertCell(1);
	cell3.innerHTML = "Total Files:" + filecount;
	cell4.innerHTML = "Total Size:" + totalsize;
}

/*
	File Input snippet
	By Osvaldas Valutis, www.osvaldas.info
	Available for use under the MIT License
*/

var inputs = document.querySelectorAll( '.inputfile' );
Array.prototype.forEach.call( inputs, function( input )
{
	var label	 = input.nextElementSibling,
		labelVal = label.innerHTML;

	input.addEventListener( 'change', function( e )
	{
		var fileName = '';
		if( this.files && this.files.length > 1 )
			fileName = ( this.getAttribute( 'data-multiple-caption' ) || '' ).replace( '{count}', this.files.length );
		else
			fileName = e.target.value.split( '\\' ).pop();

		if( fileName )
			label.querySelector( 'span' ).innerHTML = fileName;
		else
			label.innerHTML = labelVal;
	});
});


/*  Function for highlighting row of staged file */

function highlight_row() 
{
    var table = document.getElementById('table');
    var cells = table.getElementsByTagName('td');

    for (var i = 0; i < cells.length; i++) 
    {
        // Take each cell
        var cell = cells[i];
        // do something on onclick event for cell
        cell.onclick = function () 
        {
            // Get the row id where the cell exists
            var rowId = this.parentNode.rowIndex;

            var rowsNotSelected = table.getElementsByTagName('tr');
            for (var row = 0; row < rowsNotSelected.length; row++) 
            {
                rowsNotSelected[row].style.backgroundColor = "";
                rowsNotSelected[row].classList.remove('selected');
            }
            var rowSelected = table.getElementsByTagName('tr')[rowId];
            rowSelected.style.backgroundColor = "#e5a734";
            rowSelected.className += " selected";


            
        }
    }

} //end of function
document.getElementById('file-7').addEventListener('change', readSingleFile, false);

window.onload = highlight_row();

window.onload = function () 
{ 
	fetchFileList(); 
};