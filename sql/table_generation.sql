-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';

-- -----------------------------------------------------
-- Schema eco_nodes
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema eco_nodes
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `eco_nodes` DEFAULT CHARACTER SET utf8 ;
USE `eco_nodes` ;

-- -----------------------------------------------------
-- Table `eco_nodes`.`data`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `eco_nodes`.`data` (
  `node` SMALLINT UNSIGNED NOT NULL,
  `boot_count` SMALLINT UNSIGNED NOT NULL,
  `sensor` TINYINT UNSIGNED NOT NULL,
  `parameter` TINYINT UNSIGNED NOT NULL,
  `read_time` DATETIME NOT NULL COMMENT 'time when data-point was collected',
  `store_time` DATETIME GENERATED ALWAYS AS ()  COMMENT 'time when data-point was stored in server',
  `data` FLOAT NOT NULL,
  PRIMARY KEY (`node`, `boot_count`, `sensor`, `parameter`, `read_time`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eco_nodes`.`node_setup`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `eco_nodes`.`node_setup` (
  `address` SMALLINT UNSIGNED NOT NULL,
  `boot_count` SMALLINT UNSIGNED NOT NULL,
  `boot_time` DATETIME NOT NULL,
  `store_time` DATETIME GENERATED ALWAYS AS (),
  `position` POINT NULL,
  `comment` TEXT NULL,
  PRIMARY KEY (`address`, `boot_count`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eco_nodes`.`parameter`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `eco_nodes`.`parameter` (
  `node_address` INT UNSIGNED NOT NULL,
  `node_boot_count` SMALLINT UNSIGNED NOT NULL,
  `sensor` TINYINT NOT NULL,
  `parameter` TINYTEXT NOT NULL,
  `unit` TINYTEXT NULL,
  INDEX `fk_sensor_node_setup1_idx` (`node_address` ASC, `node_boot_count` ASC) VISIBLE,
  PRIMARY KEY (`sensor`, `node_address`, `node_boot_count`),
  CONSTRAINT `fk_sensor_node_setup1`
    FOREIGN KEY (`node_address` , `node_boot_count`)
    REFERENCES `eco_nodes`.`node_setup` (`address` , `boot_count`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
