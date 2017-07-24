 /*
   File Input snippet
   By Osvaldas Valutis, www.osvaldas.info
   Available for use under the MIT License
*/

var inputs = document.querySelectorAll('.inputfile');
Array.prototype.forEach.call(inputs, function(input)
{
    var label = input.nextElementSibling,
        labelVal = label.innerHTML;

    input.addEventListener('change', function(e)
    {
        var fileName = '';
        if (this.files && this.files.length > 1)
            fileName = (this.getAttribute('data-multiple-caption') || '').replace('{count}', this.files.length);
        else
            fileName = e.target.value.split('\\').pop();

        if (fileName)
            label.querySelector('span').innerHTML = fileName;
        else
            label.innerHTML = labelVal;
    });
});

/*   Highlight selected row containing file to be staged  */
$('#table').on('click', 'tbody tr', function(event) {
$(this).addClass('highlight').siblings().removeClass('highlight');
});