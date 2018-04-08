<?php


//Index.php est notre routeur, chaque requete http passera par cette page et recuperera grace a une methode GET la redirection qu'il devra effectuer. Ainsi l'utilisateur ne naviguera pas directement
// sur les pages du serveur
require_once('Controller.php');


if (isset($_GET['action'])) {// si l'on veut etre rediriger on met dans action l'attribut correspondant

    if ($_GET['action'] == 'acceuil') {

        acceuil();

    }

    elseif ($_GET['action'] == 'inscription') {
	inscription1();
        

        }
	 elseif ($_GET['action'] == 'monCompte') {

       		monCompte();

        }
	elseif ($_GET['action'] == 'deconnexion') {

       		deconnexion();

        }
	elseif ($_GET['action'] == 'verifConnection') {

       		verifConnection();

        }
	elseif ($_GET['action'] == 'modifAccount') {

       		modifAccount();

        }
	elseif ($_GET['action'] == 'modifPassword') {

	       		modifPassword();

	}
	elseif ($_GET['action'] == 'info'){
		info();
	}
        elseif($_GET['action'] == 'patho'){

	patho();
	}

}

else {// sinon par default nous irons sur la page d'acceuil du site

    acceuil();

}
?>
