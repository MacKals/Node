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
-- Table `eco_nodes`.`data_points`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `eco_nodes`.`data_points` (
  `node_id` SMALLINT UNSIGNED NOT NULL,
  `boot_count` SMALLINT UNSIGNED NOT NULL,
  `sensor_address` TINYINT UNSIGNED NOT NULL,
  `parameter_num` TINYINT UNSIGNED NOT NULL,
  `read_time` DATETIME NOT NULL,
  `store_time` DATETIME NOT NULL,
  `data_point` FLOAT NOT NULL,
  PRIMARY KEY (`node_id`, `boot_count`, `parameter_num`, `read_time`, `sensor_address`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eco_nodes`.`node_setup`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `eco_nodes`.`node_setup` (
  `node_id` SMALLINT UNSIGNED NOT NULL,
  `boot_count` SMALLINT UNSIGNED NOT NULL,
  `boot_time` DATETIME NOT NULL,
  `store_time` DATETIME NOT NULL,
  `position` POINT NULL,
  `comment` TEXT NULL,
  PRIMARY KEY (`node_id`, `boot_count`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eco_nodes`.`parameters`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `eco_nodes`.`parameters` (
  `node_id` SMALLINT UNSIGNED NOT NULL,
  `boot_count` SMALLINT UNSIGNED NOT NULL,
  `sensor_address` TINYINT NOT NULL,
  `parameter_num` TINYINT NOT NULL,
  `sensor_type` VARCHAR(2) NOT NULL,
  `sensor_id` SMALLINT NOT NULL,
  `data_title` TINYTEXT NULL,
  `unit` TINYTEXT NULL,
  INDEX `fk_sensor_node_setup1_idx` (`node_id` ASC, `boot_count` ASC),
  PRIMARY KEY (`node_id`, `boot_count`, `sensor_address`, `parameter_num`),
  CONSTRAINT `fk_sensor_node_setup1`
    FOREIGN KEY (`node_id` , `boot_count`)
    REFERENCES `eco_nodes`.`node_setup` (`node_id` , `boot_count`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eco_nodes`.`sensors`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `eco_nodes`.`sensors` (
  `sensor_type` VARCHAR(2) NOT NULL,
  `sensor_id` SMALLINT NOT NULL,
  `brand` TINYTEXT NULL,
  `description` TEXT NULL,
  PRIMARY KEY (`sensor_id`, `sensor_type`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eco_nodes`.`calibrations`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `eco_nodes`.`calibrations` (
  `sensor_sensor_id` SMALLINT NOT NULL,
  `sensor_sensor_type` VARCHAR(2) NOT NULL,
  `time` DATETIME NOT NULL,
  `minVoltage` FLOAT NULL,
  `maxVoltage` FLOAT NULL,
  `minReading` FLOAT NULL,
  `maxReading` FLOAT NULL,
  PRIMARY KEY (`time`, `sensor_sensor_type`, `sensor_sensor_id`),
  INDEX `fk_calibration_sensor1_idx` (`sensor_sensor_id` ASC, `sensor_sensor_type` ASC),
  CONSTRAINT `fk_calibration_sensor1`
    FOREIGN KEY (`sensor_sensor_id` , `sensor_sensor_type`)
    REFERENCES `eco_nodes`.`sensors` (`sensor_id` , `sensor_type`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
