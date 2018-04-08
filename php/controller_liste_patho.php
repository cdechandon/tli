<?php

session_start(); //nous permet de récuperer les éventuelles variables $_SESSION



//echo $_SESSION['login'];


require('smartyLibs/Smarty.class.php');
require('modele_liste_patho.php');
$smarty = new Smarty();

$smarty->setTemplateDir('smarty/templates');

$smarty->setCompileDir('smarty/templates_c');

$smarty->setCacheDir('smarty/cache');
$smarty->setConfigDir('smarty/configs');

/*on initialise les variables que l'on va utiliser dans la vue pour les affichages du tableau filtré*/
$afficher_mtpc="";
$afficher="";
$afficher_meridien="";
$afficher_type_patho="";
$afficher_carac="";
$afficher_mc="";
$afficher_mtp="";
$afficher_tpc="";
$barre_recherche="";

/*variables que l'on va utiliser dans les cases "filtre"*/
$case_meridien2 = case_meridien();
$smarty ->assign('liste_case_meridien',$case_meridien2);

$case_tp2 = case_type_patho();
$smarty ->assign('liste_case_tp',$case_tp2);

$case_c2 = case_carac();
$smarty ->assign('liste_case_c',$case_c2);

/*variables pour les utilisateurs connectés*/
$barre_recherche="";
$afficher_recherche="";


/*si l'utilisateur est connecté, on affiche la barre de recherche*/
if ( isset($_SESSION['pseudo'])){
$barre_recherche="  <form method=\"post\" action=\"  \" >
	<p>
		<label for=\"recherche\">Barre de recherche</label>
		<input type=\"text\" name=\"recherche\" id=\"recherche\" placeholder=\"Ex : foie\" />
		<input type=\"submit\" />
	</p>
</form>  ";

}
$smarty->assign('barre_recherche',$barre_recherche);
$smarty->assign('afficher_recherche',$afficher_recherche);

//utilisation de la barre de recherche
if (isset($_POST['recherche'])){
$afficher_recherche=recherche($_POST['recherche']);
$smarty->assign('afficher_recherche',$afficher_recherche);
}


//si on utilise pas les filtres
if ( (!isset($_POST['type_patho']) and !isset($_POST['carac']) and  !isset($_POST['nom_meridien']) and  !isset($_POST['recherche'])) or (isset($_POST['tout'])) ){
$afficher=afficher_tout();
}

$smarty->assign('afficher', $afficher);
//si on utilise les 3 filtres
if (isset($_POST['nom_meridien']) and isset($_POST['type_patho']) and isset($_POST['carac']) and $_POST['nom_meridien']!="rien" and $_POST['type_patho']!="rien" and $_POST['carac']!="rien"){
$afficher_mtpc=filtre_mer_tp_carac($_POST['nom_meridien'],$_POST['type_patho'],$_POST['carac']);
}

//si on utilise les filtres "type_patho" et "carac"
else if (isset($_POST['type_patho']) and isset($_POST['carac']) and $_POST['type_patho']!="rien" and $_POST['carac']!="rien"){
$afficher_tpc=filtre_tp_carac($_POST['type_patho'],$_POST['carac']);
}

//si on utilise les filtres "nom_meridien" et "type_patho"
else if (isset($_POST['nom_meridien']) and isset($_POST['type_patho']) and $_POST['nom_meridien']!="rien" and $_POST['type_patho']!="rien"){
$afficher_mtp=filtre_mer_tp($_POST['nom_meridien'],$_POST['type_patho']);
}

//si on utilise les filtres "nom_meridien" et "carac"
else if (isset($_POST['nom_meridien']) and isset($_POST['carac']) and $_POST['nom_meridien']!="rien" and $_POST['carac']!="rien"){
$afficher_mc=filtre_mer_carac($_POST['nom_meridien'],$_POST['carac']);
}

//si on utilise le filtre "carac"
else if (isset($_POST['carac']) and $_POST['carac']!="rien") {
$afficher_carac=filtre_carac($_POST['carac']);
}

//si on utilise le filtre "type_patho"
else if (isset($_POST['type_patho']) and $_POST['type_patho']!="rien") {
$afficher_type_patho=filtre_type_patho($_POST['type_patho']);
}

//si on utilise le filtre "nom_meridien"
else if  (isset($_POST['nom_meridien']) and $_POST['nom_meridien']!="rien") {
$afficher_meridien=filtre_meridien($_POST['nom_meridien']);
}


$smarty->assign('afficher_type_patho', $afficher_type_patho);
$smarty->assign('afficher_carac', $afficher_carac);
$smarty->assign('afficher_tpc', $afficher_tpc);
$smarty->assign('afficher_mtp', $afficher_mtp);
$smarty->assign('afficher_mc', $afficher_mc);
$smarty->assign('afficher_mtpc', $afficher_mtpc);
$smarty->assign('afficher_meridien', $afficher_meridien);


/*on affiche la vue*/
$smarty->display('../html/vue_liste_patho.html');
