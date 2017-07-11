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