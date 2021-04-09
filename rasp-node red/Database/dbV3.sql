-- Table: public.current_state

-- DROP TABLE public.current_state;

CREATE TABLE public.current_state
(
    id bigint NOT NULL DEFAULT nextval('current_state_id_seq'::regclass),
    mac character(50) COLLATE pg_catalog."default" NOT NULL,
    warehouse text COLLATE pg_catalog."default" NOT NULL,
    "ReadingTime" timestamp without time zone NOT NULL DEFAULT LOCALTIMESTAMP,
    CONSTRAINT current_state_pkey PRIMARY KEY (id),
    CONSTRAINT rela_mac FOREIGN KEY (mac)
        REFERENCES public.item (mac) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION
        NOT VALID
)

TABLESPACE pg_default;

ALTER TABLE public.current_state
    OWNER to pi;
-- Index: fki_rela_mac

-- DROP INDEX public.fki_rela_mac;

CREATE INDEX fki_rela_mac
    ON public.current_state USING btree
    (mac COLLATE pg_catalog."default" ASC NULLS LAST)
    TABLESPACE pg_default;
    
    
     
     
     
     
     
-- Table: public.item

-- DROP TABLE public.item;

CREATE TABLE public.item
(
    id bigint NOT NULL DEFAULT nextval('item_id_seq'::regclass),
    name character(50) COLLATE pg_catalog."default" NOT NULL,
    make character(50) COLLATE pg_catalog."default" NOT NULL,
    model character(50) COLLATE pg_catalog."default" NOT NULL,
    colour character(50) COLLATE pg_catalog."default" NOT NULL,
    mac character(50) COLLATE pg_catalog."default" NOT NULL,
    CONSTRAINT item_pkey PRIMARY KEY (id),
    CONSTRAINT item_mac_key UNIQUE (mac)
)

TABLESPACE pg_default;

ALTER TABLE public.item
    OWNER to pi;
    
    
    
    
    
    
    
    
-- Table: public.keep_temp

-- DROP TABLE public.keep_temp;

CREATE TABLE public.keep_temp
(
    id integer NOT NULL DEFAULT nextval('keep_temp_id_seq'::regclass),
    mac character(50) COLLATE pg_catalog."default" NOT NULL,
    ware text COLLATE pg_catalog."default" NOT NULL,
    status text COLLATE pg_catalog."default" NOT NULL DEFAULT 'keep'::text,
    CONSTRAINT keep_temp_pkey PRIMARY KEY (id)
)

TABLESPACE pg_default;

ALTER TABLE public.keep_temp
    OWNER to pi;
    
    
    
    
    
    
    
-- Table: public.records

-- DROP TABLE public.records;

CREATE TABLE public.records
(
    id bigint NOT NULL DEFAULT nextval('records_id_seq'::regclass),
    mac character(50) COLLATE pg_catalog."default" NOT NULL,
    warehouse text COLLATE pg_catalog."default",
    "ReadingTime" timestamp without time zone NOT NULL DEFAULT LOCALTIMESTAMP,
    status text COLLATE pg_catalog."default",
    CONSTRAINT records_pkey PRIMARY KEY (id)
)

TABLESPACE pg_default;

ALTER TABLE public.records
    OWNER to pi;
    
    
    
    
    
    
    
    
-- Table: public.tmp_state

-- DROP TABLE public.tmp_state;

CREATE TABLE public.tmp_state
(
    id bigint NOT NULL DEFAULT nextval('"TEMP_STATE_id_seq"'::regclass),
    mac character(50) COLLATE pg_catalog."default" DEFAULT NULL::bpchar,
    warehouse text COLLATE pg_catalog."default",
    CONSTRAINT "TEMP_STATE_pkey" PRIMARY KEY (id)
)

TABLESPACE pg_default;

ALTER TABLE public.tmp_state
    OWNER to pi;

-- Trigger: tmp_control

-- DROP TRIGGER tmp_control ON public.tmp_state;

CREATE TRIGGER tmp_control
    AFTER INSERT
    ON public.tmp_state
    FOR EACH STATEMENT
    EXECUTE PROCEDURE public.stock_control();
    
    
    
    
    
    
    
    
    
    
    
    
    
-- FUNCTION: public.stock_control()

-- DROP FUNCTION public.stock_control();

CREATE FUNCTION public.stock_control()
    RETURNS trigger
    LANGUAGE 'plpgsql'
    COST 100
    VOLATILE NOT LEAKPROOF
AS $BODY$
Begin
IF Exists(Select 1 from public.tmp_state where tmp_state.mac IS NULL) Then
	
	insert into public.keep_temp (mac, ware)
	select current_state.mac, current_state.warehouse from public.current_state
	where not exists (Select current_state.mac, current_state.warehouse from public.tmp_state 
	WHERE current_state.mac = tmp_state.mac);

	Update public.keep_temp set status = 'out';

	insert into public.records(mac, warehouse, status) 
	select keep_temp.mac, keep_temp.ware, keep_temp.status from public.keep_temp;

	TRUNCATE public.keep_temp
	RESTART IDENTITY;
	
	TRUNCATE public.current_state
	RESTART IDENTITY; 

ELSE

	insert into public.keep_temp (mac, ware)
	select tmp_state.mac, tmp_state.warehouse from public.tmp_state
	where not exists (Select tmp_state.mac, tmp_state.warehouse from public.current_state 
	WHERE tmp_state.mac = current_state.mac) ;

	Update public.keep_temp set status = 'in';

	insert into public.records(mac, warehouse, status) 
	select keep_temp.mac, keep_temp.ware, keep_temp.status from public.keep_temp;

	TRUNCATE public.keep_temp
	RESTART IDENTITY; 	

	insert into public.keep_temp (mac, ware)
	select current_state.mac, current_state.warehouse from public.current_state
	where not exists (Select current_state.mac, current_state.warehouse from public.tmp_state 
	WHERE current_state.mac = tmp_state.mac) ;

	Update public.keep_temp set status = 'out';

	insert into public.records(mac, warehouse, status) 
	select keep_temp.mac, keep_temp.ware, keep_temp.status from public.keep_temp;

	TRUNCATE public.keep_temp
	RESTART IDENTITY; 	

	insert into public.keep_temp (mac, ware)
	select current_state.mac, current_state.warehouse from public.current_state, public.tmp_state 
	WHERE current_state.mac = tmp_state.mac 
	AND (
	 current_state.warehouse = tmp_state.warehouse 
	);

	 
	insert into public.records(mac, warehouse, status) 
	select keep_temp.mac, keep_temp.ware, keep_temp.status from public.keep_temp;

	TRUNCATE public.keep_temp
	RESTART IDENTITY;

	TRUNCATE public.current_state
	RESTART IDENTITY; 

	insert into public.current_state(mac, warehouse) 
	select tmp_state.mac, tmp_state.warehouse from public.tmp_state;

END IF;

RETURN NULL;

END
$BODY$;

ALTER FUNCTION public.stock_control()
    OWNER TO pi;

