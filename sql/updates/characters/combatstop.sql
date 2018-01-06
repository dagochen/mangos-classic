/*
SQLyog Community v12.5.0 (64 bit)
MySQL - 5.7.20-log 
*********************************************************************
*/
/*!40101 SET NAMES utf8 */;

create table `raidstats_combatstop` (
	`id` int (11),
	`timestamp` datetime ,
	`raidId` int (11),
	`bossGuid` int (11),
	`playerGuid` int (11),
	`zone` int (11)
); 
