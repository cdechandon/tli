<?php

//Controller .php sert de redirection vers les bonnes pages .php, cela permet de rajouter une securite grace a ce routeur.
require_once('model.php');


function acceuil(){


    require('IndexAccueil.php');

}


function inscription1(){


    require('IndexInscription.php');

}

function monCompte(){


	require('myAccount.php');

}
function deconnexion(){


	require('Deconnexion.php');

}

function verifConnection(){
	require('ConnexionVerif.php');

	
	}

function modifAccount(){
	require('ModificationAccountGeneral.php');
}
function modifPassword(){
	require('ModificationAccountPassword.php');
}
function info(){
	require('Info.php');
}

function patho(){
	require('controller_liste_patho.php');
}
?>
