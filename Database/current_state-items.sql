



create table current_state (
	id BIGSERIAL NOT NULL PRIMARY KEY,
	mac VARCHAR(50) NOT NULL,
	warehouse text,
	ReadingTime timestamp NOT NULL DEFAULT NOW()
);

create table item (
	id BIGSERIAL NOT NULL PRIMARY KEY,
	name VARCHAR(100) NOT NULL,
	make VARCHAR(100),
	model VARCHAR(100),
	colour VARCHAR(50),
	mac_id BIGINT REFERENCES current_state(id),
	UNIQUE(mac_id)
);

