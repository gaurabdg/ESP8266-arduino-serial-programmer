var fName = null;
var fData = null;
var espURL = "http://" + location.host;

function UploadData()
{
	var xhr = new XMLHttpRequest();
	var url = espUrl + "/upload&"+ fName
	var params = fData;
	xhr.open("POST", url, true);
	xhr.send(params);
}

function UploadFile() 
{
	UploadData();
	cancelUpload();
	setTimeout(fetchFileList, 2000);
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
			showFiles(xhr.responseText);
   		} 
   		else 
   		{
			showFilesError();
		}
	}
	
	xhr.send();
}

function deleteFile(filename) 
{
	var xhr = new XMLHttpRequest();
	var url = espUrl + "/delete&" + fName;

	xhr.open("GET", url, true);
	xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	xhr.onreadystatechange = function() 
	{
		if(xhr.readyState == 4 && xhr.status == 200) 
		{
			showFiles(xhr.responseText);
   		} 
   		else 
   		{
			showFilesError();
		}
	}
	xhr.send();
}

function flashFile(filename) 
{
	var xhr = new XMLHttpRequest();
	var url = espUrl + "/flash&" + fName;

	xhr.open("GET", url, true);
	xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	xhr.onreadystatechange = function() 
	{
		if(xhr.readyState == 4 && xhr.status == 200) 
		{
			showFiles(xhr.responseText);
   		}
	}
	xhr.send();
}

function getFileRow(filename, filesize) 
{
	var row = document.createElement("div");
	row.id = "Row";
	row.appendChild(getTableCell(filename, "W200"));
	row.appendChild(getTableCell(filesize, "W100"));
	row.appendChild(getCmdOption("Flash", "W50", filename));
	row.appendChild(getCmdOption("&nbsp;", "W50", filename));
	row.appendChild(getCmdOption("Delete", "W50", filename));

	return row;
}
