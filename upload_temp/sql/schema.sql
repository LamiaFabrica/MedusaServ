-- PFQL Portal Database Schema
-- Yorkshire Champion Gold Standard Database Design
-- Compatible with PostgreSQL 13+

-- ============================================================================
-- PFQL SERVICE REGISTRY TABLES
-- ============================================================================

-- Service Registry Table
CREATE TABLE IF NOT EXISTS pfql_service_registry (
    id SERIAL PRIMARY KEY,
    service_id VARCHAR(255) UNIQUE NOT NULL,
    name VARCHAR(255) NOT NULL,
    category VARCHAR(100) NOT NULL,
    description TEXT,
    version VARCHAR(50) DEFAULT '1.0.0',
    file_path VARCHAR(500) NOT NULL,
    api_routes JSONB,
    dependencies JSONB DEFAULT '[]',
    config JSONB DEFAULT '{}',
    status VARCHAR(50) DEFAULT 'active',
    created_at TIMESTAMP DEFAULT NOW(),
    updated_at TIMESTAMP DEFAULT NOW()
);

-- Service Categories Table
CREATE TABLE IF NOT EXISTS pfql_service_categories (
    id SERIAL PRIMARY KEY,
    category_id VARCHAR(100) UNIQUE NOT NULL,
    name VARCHAR(255) NOT NULL,
    description TEXT,
    icon VARCHAR(50),
    sort_order INTEGER DEFAULT 0,
    created_at TIMESTAMP DEFAULT NOW()
);

-- Service Metrics Table
CREATE TABLE IF NOT EXISTS pfql_service_metrics (
    id SERIAL PRIMARY KEY,
    service_id VARCHAR(255) NOT NULL,
    metric_name VARCHAR(100) NOT NULL,
    metric_value NUMERIC,
    metric_data JSONB,
    timestamp TIMESTAMP DEFAULT NOW(),
    FOREIGN KEY (service_id) REFERENCES pfql_service_registry(service_id) ON DELETE CASCADE
);

-- ============================================================================
-- PFQL AUTHENTICATION AND USER MANAGEMENT
-- ============================================================================

-- User Accounts Table
CREATE TABLE IF NOT EXISTS pfql_users (
    id SERIAL PRIMARY KEY,
    username VARCHAR(255) UNIQUE NOT NULL,
    email VARCHAR(255) UNIQUE,
    password_hash VARCHAR(255) NOT NULL,
    role VARCHAR(50) DEFAULT 'user',
    permissions JSONB DEFAULT '[]',
    yorkshire_access BOOLEAN DEFAULT FALSE,
    master_access BOOLEAN DEFAULT FALSE,
    status VARCHAR(50) DEFAULT 'active',
    last_login TIMESTAMP,
    created_at TIMESTAMP DEFAULT NOW(),
    updated_at TIMESTAMP DEFAULT NOW()
);

-- User Sessions Table
CREATE TABLE IF NOT EXISTS pfql_user_sessions (
    id SERIAL PRIMARY KEY,
    user_id INTEGER NOT NULL,
    session_token VARCHAR(500) UNIQUE NOT NULL,
    expires_at TIMESTAMP NOT NULL,
    ip_address INET,
    user_agent TEXT,
    yorkshire_session BOOLEAN DEFAULT FALSE,
    created_at TIMESTAMP DEFAULT NOW(),
    FOREIGN KEY (user_id) REFERENCES pfql_users(id) ON DELETE CASCADE
);

-- ============================================================================
-- PFQL EXECUTION AND AUDIT LOGGING
-- ============================================================================

-- PFQL Command Execution Log
CREATE TABLE IF NOT EXISTS pfql_execution_log (
    id SERIAL PRIMARY KEY,
    user_id INTEGER,
    session_id INTEGER,
    command VARCHAR(500) NOT NULL,
    options JSONB,
    result JSONB,
    execution_time_ms INTEGER,
    status VARCHAR(50) DEFAULT 'success',
    error_message TEXT,
    timestamp TIMESTAMP DEFAULT NOW(),
    FOREIGN KEY (user_id) REFERENCES pfql_users(id) ON DELETE SET NULL,
    FOREIGN KEY (session_id) REFERENCES pfql_user_sessions(id) ON DELETE SET NULL
);

-- PFQL Error Log
CREATE TABLE IF NOT EXISTS pfql_error_log (
    id SERIAL PRIMARY KEY,
    user_id INTEGER,
    session_id INTEGER,
    command VARCHAR(500) NOT NULL,
    options JSONB,
    error TEXT NOT NULL,
    stack_trace TEXT,
    timestamp TIMESTAMP DEFAULT NOW(),
    FOREIGN KEY (user_id) REFERENCES pfql_users(id) ON DELETE SET NULL,
    FOREIGN KEY (session_id) REFERENCES pfql_user_sessions(id) ON DELETE SET NULL
);

-- ============================================================================
-- SYSTEM MONITORING AND HEALTH TABLES
-- ============================================================================

-- System Health Metrics
CREATE TABLE IF NOT EXISTS pfql_system_health (
    id SERIAL PRIMARY KEY,
    server_id VARCHAR(255) NOT NULL,
    cpu_usage NUMERIC(5,2),
    memory_usage NUMERIC(5,2),
    disk_usage NUMERIC(5,2),
    network_latency INTEGER,
    active_connections INTEGER,
    response_time_ms INTEGER,
    error_rate NUMERIC(5,4),
    overall_score NUMERIC(5,2),
    status VARCHAR(50) DEFAULT 'healthy',
    timestamp TIMESTAMP DEFAULT NOW()
);

-- Database Performance Metrics
CREATE TABLE IF NOT EXISTS pfql_database_metrics (
    id SERIAL PRIMARY KEY,
    database_name VARCHAR(255) NOT NULL,
    connections_active INTEGER,
    connections_idle INTEGER,
    query_count INTEGER,
    avg_query_time_ms NUMERIC(10,3),
    cache_hit_ratio NUMERIC(5,4),
    locks_waiting INTEGER,
    deadlocks INTEGER,
    size_bytes BIGINT,
    timestamp TIMESTAMP DEFAULT NOW()
);

-- ============================================================================
-- SECURITY AND VULNERABILITY MANAGEMENT
-- ============================================================================

-- Vulnerability Scan Results
CREATE TABLE IF NOT EXISTS pfql_vulnerability_scans (
    id SERIAL PRIMARY KEY,
    scan_id VARCHAR(255) UNIQUE NOT NULL,
    scan_type VARCHAR(100) NOT NULL,
    target_systems TEXT[],
    scan_status VARCHAR(50) DEFAULT 'running',
    vulnerabilities_found INTEGER DEFAULT 0,
    critical_count INTEGER DEFAULT 0,
    high_count INTEGER DEFAULT 0,
    medium_count INTEGER DEFAULT 0,
    low_count INTEGER DEFAULT 0,
    scan_results JSONB,
    started_at TIMESTAMP DEFAULT NOW(),
    completed_at TIMESTAMP
);

-- Security Events Log
CREATE TABLE IF NOT EXISTS pfql_security_events (
    id SERIAL PRIMARY KEY,
    event_type VARCHAR(100) NOT NULL,
    severity VARCHAR(50) NOT NULL,
    source_ip INET,
    user_id INTEGER,
    description TEXT NOT NULL,
    event_data JSONB,
    resolved BOOLEAN DEFAULT FALSE,
    timestamp TIMESTAMP DEFAULT NOW(),
    FOREIGN KEY (user_id) REFERENCES pfql_users(id) ON DELETE SET NULL
);

-- ============================================================================
-- PERFORMANCE BENCHMARKING
-- ============================================================================

-- Performance Benchmark Results
CREATE TABLE IF NOT EXISTS pfql_benchmark_results (
    id SERIAL PRIMARY KEY,
    benchmark_id VARCHAR(255) UNIQUE NOT NULL,
    benchmark_type VARCHAR(100) NOT NULL,
    system_config JSONB,
    cpu_score INTEGER,
    memory_score INTEGER,
    disk_score INTEGER,
    network_score INTEGER,
    overall_score INTEGER,
    benchmark_data JSONB,
    duration_ms INTEGER,
    timestamp TIMESTAMP DEFAULT NOW()
);

-- ============================================================================
-- CHAOS ENGINEERING
-- ============================================================================

-- Chaos Test Results
CREATE TABLE IF NOT EXISTS pfql_chaos_tests (
    id SERIAL PRIMARY KEY,
    test_id VARCHAR(255) UNIQUE NOT NULL,
    test_type VARCHAR(100) NOT NULL,
    target_service VARCHAR(255),
    test_parameters JSONB,
    test_results JSONB,
    system_response VARCHAR(100),
    recovery_time_ms INTEGER,
    success BOOLEAN DEFAULT TRUE,
    started_at TIMESTAMP DEFAULT NOW(),
    completed_at TIMESTAMP
);

-- ============================================================================
-- AI AND CONSCIOUSNESS METRICS
-- ============================================================================

-- Consciousness Awareness Metrics
CREATE TABLE IF NOT EXISTS pfql_consciousness_metrics (
    id SERIAL PRIMARY KEY,
    session_id VARCHAR(255) NOT NULL,
    awareness_level NUMERIC(5,2),
    consciousness_depth NUMERIC(5,2),
    enlightenment_score NUMERIC(5,2),
    transcendence_state NUMERIC(5,2),
    operation_type VARCHAR(100),
    insights TEXT[],
    timestamp TIMESTAMP DEFAULT NOW()
);

-- AI Model Performance
CREATE TABLE IF NOT EXISTS pfql_ai_metrics (
    id SERIAL PRIMARY KEY,
    model_name VARCHAR(255) NOT NULL,
    operation_type VARCHAR(100) NOT NULL,
    input_size INTEGER,
    processing_time_ms INTEGER,
    accuracy_score NUMERIC(5,4),
    confidence_level NUMERIC(5,4),
    resource_usage JSONB,
    timestamp TIMESTAMP DEFAULT NOW()
);

-- ============================================================================
-- QUANTUM COMPUTING METRICS
-- ============================================================================

-- Quantum Optimization Results
CREATE TABLE IF NOT EXISTS pfql_quantum_results (
    id SERIAL PRIMARY KEY,
    optimization_id VARCHAR(255) UNIQUE NOT NULL,
    algorithm_type VARCHAR(100) NOT NULL,
    qubits_used INTEGER,
    circuit_depth INTEGER,
    fidelity NUMERIC(6,4),
    coherence_time_ms INTEGER,
    optimization_gain NUMERIC(8,4),
    quantum_advantage JSONB,
    timestamp TIMESTAMP DEFAULT NOW()
);

-- ============================================================================
-- PORTAL CONFIGURATION AND SETTINGS
-- ============================================================================

-- Portal Configuration Settings
CREATE TABLE IF NOT EXISTS pfql_portal_config (
    id SERIAL PRIMARY KEY,
    config_key VARCHAR(255) UNIQUE NOT NULL,
    config_value JSONB NOT NULL,
    description TEXT,
    category VARCHAR(100),
    is_sensitive BOOLEAN DEFAULT FALSE,
    created_at TIMESTAMP DEFAULT NOW(),
    updated_at TIMESTAMP DEFAULT NOW()
);

-- Portal Activity Log
CREATE TABLE IF NOT EXISTS pfql_portal_logs (
    id SERIAL PRIMARY KEY,
    user_id INTEGER,
    session_id INTEGER,
    action VARCHAR(255) NOT NULL,
    resource VARCHAR(255),
    details JSONB,
    ip_address INET,
    user_agent TEXT,
    timestamp TIMESTAMP DEFAULT NOW(),
    FOREIGN KEY (user_id) REFERENCES pfql_users(id) ON DELETE SET NULL,
    FOREIGN KEY (session_id) REFERENCES pfql_user_sessions(id) ON DELETE SET NULL
);

-- ============================================================================
-- INDEXES FOR PERFORMANCE OPTIMIZATION
-- ============================================================================

-- Service Registry Indexes
CREATE INDEX IF NOT EXISTS idx_pfql_service_registry_category ON pfql_service_registry(category);
CREATE INDEX IF NOT EXISTS idx_pfql_service_registry_status ON pfql_service_registry(status);

-- Metrics Indexes
CREATE INDEX IF NOT EXISTS idx_pfql_service_metrics_service_id ON pfql_service_metrics(service_id);
CREATE INDEX IF NOT EXISTS idx_pfql_service_metrics_timestamp ON pfql_service_metrics(timestamp DESC);
CREATE INDEX IF NOT EXISTS idx_pfql_system_health_timestamp ON pfql_system_health(timestamp DESC);
CREATE INDEX IF NOT EXISTS idx_pfql_database_metrics_timestamp ON pfql_database_metrics(timestamp DESC);

-- Authentication Indexes
CREATE INDEX IF NOT EXISTS idx_pfql_users_username ON pfql_users(username);
CREATE INDEX IF NOT EXISTS idx_pfql_users_email ON pfql_users(email);
CREATE INDEX IF NOT EXISTS idx_pfql_user_sessions_token ON pfql_user_sessions(session_token);
CREATE INDEX IF NOT EXISTS idx_pfql_user_sessions_expires ON pfql_user_sessions(expires_at);

-- Logging Indexes
CREATE INDEX IF NOT EXISTS idx_pfql_execution_log_timestamp ON pfql_execution_log(timestamp DESC);
CREATE INDEX IF NOT EXISTS idx_pfql_execution_log_user_id ON pfql_execution_log(user_id);
CREATE INDEX IF NOT EXISTS idx_pfql_error_log_timestamp ON pfql_error_log(timestamp DESC);
CREATE INDEX IF NOT EXISTS idx_pfql_portal_logs_timestamp ON pfql_portal_logs(timestamp DESC);

-- Security Indexes
CREATE INDEX IF NOT EXISTS idx_pfql_security_events_timestamp ON pfql_security_events(timestamp DESC);
CREATE INDEX IF NOT EXISTS idx_pfql_security_events_severity ON pfql_security_events(severity);

-- ============================================================================
-- INITIAL DATA INSERTS
-- ============================================================================

-- Insert default service categories
INSERT INTO pfql_service_categories (category_id, name, description, icon, sort_order) VALUES
('database', '🗄️ Database Services', 'Database management and optimization services', '🗄️', 1),
('system', '⚙️ System Services', 'System monitoring and management services', '⚙️', 2),
('performance', '🚀 Performance Services', 'Performance analysis and optimization services', '🚀', 3),
('security', '🛡️ Security Services', 'Security and threat management services', '🛡️', 4),
('quantum', '⚛️ Quantum Services', 'Quantum computing and optimization services', '⚛️', 5),
('chaos', '🔥 Chaos Services', 'Chaos engineering and resilience services', '🔥', 6),
('consciousness', '🧠 Consciousness Services', 'AI consciousness and awareness services', '🧠', 7),
('ai', '🤖 AI Services', 'Artificial intelligence and automation services', '🤖', 8)
ON CONFLICT (category_id) DO NOTHING;

-- Insert default master user (LamiaFabrica)
INSERT INTO pfql_users (username, email, password_hash, role, permissions, yorkshire_access, master_access, status) VALUES
('LamiaFabrica', 'master@poweredbymedusa.com', 
 '$2b$12$b06874c4e4534e412d54e8b23273b49544d104b41ee8d7f6fc0ccb128c2bef21', 
 'root', 
 '["*"]', 
 TRUE, 
 TRUE, 
 'active')
ON CONFLICT (username) DO NOTHING;

-- Insert portal configuration defaults
INSERT INTO pfql_portal_config (config_key, config_value, description, category) VALUES
('portal.name', '"PFQL Portal"', 'Portal application name', 'general'),
('portal.version', '"1.0.0"', 'Portal version number', 'general'),
('yorkshire.standards.enabled', 'true', 'Enable Yorkshire Champion standards', 'yorkshire'),
('yorkshire.master.key', '"b06874c4e4534e412d54e8b23273b49544d104b41ee8d7f6fc0ccb128c2bef21"', 'Yorkshire Champion master key', 'yorkshire'),
('pfql.command.timeout', '30000', 'Default PFQL command timeout in milliseconds', 'pfql'),
('monitoring.health.interval', '30000', 'Health check interval in milliseconds', 'monitoring'),
('security.encryption.enabled', 'true', 'Enable data encryption', 'security')
ON CONFLICT (config_key) DO NOTHING;

-- ============================================================================
-- FUNCTIONS AND TRIGGERS
-- ============================================================================

-- Update timestamp function
CREATE OR REPLACE FUNCTION update_updated_at_column()
RETURNS TRIGGER AS $$
BEGIN
    NEW.updated_at = NOW();
    RETURN NEW;
END;
$$ language 'plpgsql';

-- Update triggers for timestamp columns
CREATE TRIGGER update_pfql_service_registry_updated_at 
    BEFORE UPDATE ON pfql_service_registry 
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

CREATE TRIGGER update_pfql_users_updated_at 
    BEFORE UPDATE ON pfql_users 
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

CREATE TRIGGER update_pfql_portal_config_updated_at 
    BEFORE UPDATE ON pfql_portal_config 
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

-- ============================================================================
-- VIEWS FOR COMMON QUERIES
-- ============================================================================

-- Service Registry with Category Information
CREATE OR REPLACE VIEW pfql_services_full AS
SELECT 
    s.*,
    c.name as category_name,
    c.icon as category_icon,
    c.sort_order as category_sort_order
FROM pfql_service_registry s
LEFT JOIN pfql_service_categories c ON s.category = c.category_id;

-- System Health Summary
CREATE OR REPLACE VIEW pfql_system_health_summary AS
SELECT 
    server_id,
    AVG(overall_score) as avg_score,
    AVG(cpu_usage) as avg_cpu,
    AVG(memory_usage) as avg_memory,
    AVG(disk_usage) as avg_disk,
    COUNT(*) as measurement_count,
    MAX(timestamp) as last_update
FROM pfql_system_health
WHERE timestamp >= NOW() - INTERVAL '1 hour'
GROUP BY server_id;

-- User Activity Summary  
CREATE OR REPLACE VIEW pfql_user_activity_summary AS
SELECT 
    u.username,
    u.role,
    COUNT(l.id) as total_actions,
    MAX(l.timestamp) as last_activity,
    COUNT(DISTINCT DATE(l.timestamp)) as active_days
FROM pfql_users u
LEFT JOIN pfql_portal_logs l ON u.id = l.user_id
WHERE l.timestamp >= NOW() - INTERVAL '30 days'
GROUP BY u.id, u.username, u.role;

-- ============================================================================
-- GRANTS AND PERMISSIONS
-- ============================================================================

-- Grant permissions to LamiaFabrica user
GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO "LamiaFabrica";
GRANT ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public TO "LamiaFabrica";
GRANT ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public TO "LamiaFabrica";

-- Grant select permissions for monitoring user (if exists)
DO $$
BEGIN
    IF EXISTS (SELECT 1 FROM pg_roles WHERE rolname = 'pfql_monitor') THEN
        GRANT SELECT ON ALL TABLES IN SCHEMA public TO pfql_monitor;
    END IF;
END
$$;

COMMENT ON SCHEMA public IS 'PFQL Portal Yorkshire Champion Database Schema v1.0.0';