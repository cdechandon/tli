//$('#cardFamilyImgId')[0].src=document.getElementById('text');
function check() {
  
  cardName=$('#name')[0].val();
  cardDescription=$('#description')[0].val();
  cardImgURL=$('#imgurl')[0].val();
  
  cardFamily=$('#family')[0].val(); //:ou .text() si ça marche pas
  
  cardHP=$('#hp')[0].val();
  cardEnergy=$('#energy')[0].val();
  cardAttack=$('#attack')[0].val();
  cardDefence=$('#defence')[0].val();
  
  alert("name:"+cardName, "description:"+cardDescription, "family:"+ cardFamily, "hp:"+cardHP, "energy:"+cardEnergy, "attack:"+cardAttack, "defence:"+cardDefence );
  
  jsonResult={"name":cardName,"description":cardDescription "family": cardFamily, "hp":cardHP, "energy":cardEnergy, "attack":cardAttack, "defence":card};
  
  //send to server
  $.ajax({
        type: "POST",
        url: "localhost:8080/WebServiceCard/add",
        data: JSON.stringify(jsonResult),
        contentType: "application/json; charset=utf-8",
        dataType: "json",
        success: function(data){alert(data);},
        failure: function(errMsg) {
            alert(errMsg);
        }
  });
  

/*$('#cardFamilyNameId')[0].innerText=document.getElementById('name');
$('#cardImgId')[0].src="http://www.guinnessworldrecords.com/images/superlative/superheroes/GWR-Superheroes-SUPERMAN.svg";
$('#cardNameId')[0].innerText="SUPERMAN";
$('#cardDescriptionId')[0].innerText="The origin story of Superman relates that he was born Kal-El on the planet Krypton, before being rocketed to Earth as an infant by his scientist father Jor-El, moments before Krypton's destruction. Discovered and adopted by a farm couple from Kansas, the child is raised as Clark Kent and imbued with a strong moral compass. Early in his childhood, he displays various superhuman abilities, which, upon reaching maturity, he resolves to use for the benefit of humanity through a 'Superman' identity.";
$('#cardHPId')[0].innerText="50 HP";
$('#cardEnergyId')[0].innerText="100 Energy";
$('#cardAttackId')[0].innerText="17 Attack";
$('#cardDefenceId')[0].innerText="80 Defence";*/
