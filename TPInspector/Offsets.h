/*
	Treasure Planet: Battle at Procyon Inspector - mesh viewer and editor.

	*** by InkySka ***

	e-mail: underscore.marczero@gmail.com
	Twitter: @InkySka

	Official repository: https://github.com/InkySka/TPBAPMeshViewer
	.mdb files info: https://treasure-planet.wikia.com/wiki/.mdb_mesh_files
	Note: reverse-engineering is still undergoing and any help is appreciated.

	Treasure Planet Wikia: https://treasure-planet.wikia.com/

	(C) InkySka 2018 - published under the MIT Licence.

	Offsets file: some known hard-coded offsets.
*/

#pragma once

//offsets

const unsigned int O_final_strings(	0x0);
const unsigned int O_mesh_type(		0xC); //1: no collision. 2: collision
const unsigned int O_vertices(		0x18);