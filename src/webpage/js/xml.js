// declarations
var filename = null;
var fileData = null;
var fileStg = null;
var host = location.host;
var espUrl = "http://" + host;



function readSingleFile(e)
{
   var file = e.target.files[0]; //read first incoming file
   filename = file.name;
   if (!file)
   {
       return;
   }
   var reader = new FileReader();
   reader.onload = function(e)
   {
       fileData = e.target.result;
   };
   reader.readAsText(file);

   document.getElementById("up").addEventListener("click", UploadFile, false); // to add onclick() to Upload button
}


//append table rows
function GenerateRows(filename, filesize)
{

   var table = document.getElementById("table");

   var count = table.rows.length;
   var row = table.insertRow(count);

   var cell1 = row.insertCell(0);
   var cell2 = row.insertCell(1);
   cell1.innerHTML = filename;
   cell2.innerHTML = filesize;


}


function DisplayDirectory(requestList)
{
   var listData = requestList.split("\n"); // decode
   if (listData.length == 0)
   {
       alert("No data");
   }



   for (var x = 0; x < listData.length; x++)
   {
       if (listData[x].length < 5)
       {
           continue;
       }
       // parse
       var elements = listData[x].split(";");

       var str = elements[0];
       if (str.startsWith("/hex")) // prevent displaying garbage
       {
           GenerateRows(elements[0], elements[1]);
       }

   }


}

// current files in the file system
function fetchFileList()
{
   var xhr = new XMLHttpRequest();
   var url = espUrl + "/files";

   xhr.open("GET", url, true);
   xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
   xhr.onreadystatechange = function()
   {
       if (xhr.readyState == 4 && xhr.status == 200) //done & success
       {
           DisplayDirectory(xhr.responseText);

       }
       else
       {
           alert(lite);
       }

   }

   xhr.send();
}

function UploadData()
{
   var xhr = new XMLHttpRequest();
   var url = espUrl + "/upload&" + filename;
   var params = fileData;
   xhr.open("POST", url, true);
   xhr.send(params);
}

function UploadFile()
{
   UploadData();
   setTimeout(fetchFileList, 2000);

}

function deleteFile(filename)
{
   var xhr = new XMLHttpRequest();
   var url = espUrl + "/delete&" + filename;

   xhr.open("GET", url, true);
   xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
   xhr.onreadystatechange = function()
   {
       if (xhr.readyState == 4 && xhr.status == 200)
       {
           DisplayDirectory(xhr.responseText);
       }
       else
       {
           aler(lite);
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
       if (xhr.readyState == 4 && xhr.status == 200)
       {
           DisplayDirectory(xhr.responseText);
       }
   }
   xhr.send();

}

// stage selected file for flasing/deleting
function StageFile(e)
{
   fileStg = e.target.innerText;
   alert(fileStg + " is staged to be flashed/deleted");

}
// global listeners
document.getElementById("flash").addEventListener("click", function()
{
   flashFile(fileStg);

});

document.getElementById("delete").addEventListener("click", function()
{
   deleteFile(fileStg);
});

document.getElementById('file-7').addEventListener('change', readSingleFile, false);

window.onload = function()
{
   fetchFileList();
};